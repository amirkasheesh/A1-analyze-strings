import os

import matplotlib.pyplot as plt
import pandas as pd


RESULTS_FILE = "src/results.csv"
OUTPUT_DIR = "docs_results"

TIME_COLUMN = "time_us"
COMPARISONS_COLUMN = "char_comparisons"


CATEGORY_NAMES = {
    "Random": "случайный массив",
    "ReverseSorted": "обратно отсортированный массив",
    "AlmostSorted": "почти отсортированный массив",
    "CommonPrefix": "массив с общими префиксами",
    "random": "случайный массив",
    "reverse": "обратно отсортированный массив",
    "almost": "почти отсортированный массив",
    "prefix": "массив с общими префиксами",
}


def prepare_output_dir():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)


def read_results():
    if not os.path.exists(RESULTS_FILE):
        raise FileNotFoundError(f"Файл не найден: {RESULTS_FILE}")

    data = pd.read_csv(RESULTS_FILE)

    data.columns = data.columns.str.strip()

    data["algorithm"] = data["algorithm"].astype(str).str.strip()
    data["category"] = data["category"].astype(str).str.strip()
    data["size"] = pd.to_numeric(data["size"])
    data[TIME_COLUMN] = pd.to_numeric(data[TIME_COLUMN])
    data[COMPARISONS_COLUMN] = pd.to_numeric(data[COMPARISONS_COLUMN])

    print("Файл results.csv успешно прочитан")
    print(f"Количество строк: {len(data)}")
    print("Найденные алгоритмы:")
    print(sorted(data["algorithm"].unique()))
    print("Найденные типы данных:")
    print(sorted(data["category"].unique()))
    print("Найденные размеры массивов:")
    print(sorted(data["size"].unique()))
    print()

    return data


def get_average_results(data):
    average_data = data.groupby(
        ["algorithm", "category", "size"],
        as_index=False
    ).agg({
        TIME_COLUMN: "mean",
        COMPARISONS_COLUMN: "mean",
    })

    return average_data


def get_category_name(category):
    if category in CATEGORY_NAMES:
        return CATEGORY_NAMES[category]

    return category


def make_file_name(category, metric_name):
    result = category.lower()
    result = result.replace(" ", "_")
    result = result.replace("-", "_")

    return f"{result}_{metric_name}.png"


def plot_metric(data, category, metric_column, title, ylabel, output_file):
    plt.figure(figsize=(12, 7))

    category_data = data[data["category"] == category]
    algorithms = sorted(category_data["algorithm"].unique())

    for algorithm in algorithms:
        algorithm_data = category_data[category_data["algorithm"] == algorithm]
        algorithm_data = algorithm_data.sort_values("size")

        if algorithm_data.empty:
            continue

        plt.plot(
            algorithm_data["size"],
            algorithm_data[metric_column],
            marker="o",
            linewidth=1.5,
            markersize=3,
            label=algorithm
        )

    plt.title(title)
    plt.xlabel("Размер массива")
    plt.ylabel(ylabel)
    plt.grid(True)

    if len(algorithms) > 0:
        plt.legend()

    plt.tight_layout()

    output_path = os.path.join(OUTPUT_DIR, output_file)
    plt.savefig(output_path, dpi=200)
    plt.close()


def plot_all_time_graphs(data):
    categories = sorted(data["category"].unique())

    for category in categories:
        category_name = get_category_name(category)
        output_file = make_file_name(category, "time")

        plot_metric(
            data=data,
            category=category,
            metric_column=TIME_COLUMN,
            title=f"Время работы алгоритмов: {category_name}",
            ylabel="Время, мкс",
            output_file=output_file
        )


def plot_all_comparison_graphs(data):
    categories = sorted(data["category"].unique())

    for category in categories:
        category_name = get_category_name(category)
        output_file = make_file_name(category, "comparisons")

        plot_metric(
            data=data,
            category=category,
            metric_column=COMPARISONS_COLUMN,
            title=f"Количество посимвольных сравнений: {category_name}",
            ylabel="Количество посимвольных сравнений",
            output_file=output_file
        )


def print_summary(data):
    max_size = data["size"].max()
    max_size_data = data[data["size"] == max_size]

    print(f"Итоговая сводка для размера массива {max_size}")
    print()

    categories = sorted(max_size_data["category"].unique())

    for category in categories:
        print(f"Тип данных: {get_category_name(category)}")

        category_data = max_size_data[max_size_data["category"] == category]

        time_sorted = category_data.sort_values(TIME_COLUMN)
        comparisons_sorted = category_data.sort_values(COMPARISONS_COLUMN)

        print("Алгоритмы по возрастанию среднего времени работы:")
        for _, row in time_sorted.iterrows():
            print(f"  {row['algorithm']}: {row[TIME_COLUMN]:.2f} мкс")

        print("Алгоритмы по возрастанию количества посимвольных сравнений:")
        for _, row in comparisons_sorted.iterrows():
            print(f"  {row['algorithm']}: {row[COMPARISONS_COLUMN]:.2f}")

        print()


def main():
    prepare_output_dir()

    data = read_results()

    if data.empty:
        raise RuntimeError("Файл results.csv пустой. Сначала запусти main.cpp и получи результаты.")

    average_data = get_average_results(data)

    plot_all_time_graphs(average_data)
    plot_all_comparison_graphs(average_data)

    print_summary(average_data)

    print("Графики сохранены в папку docs_results")


if __name__ == "__main__":
    main()