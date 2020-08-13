import matplotlib.pyplot as plt
from argparse import ArgumentParser
import json

def plot_paths(paths: list):
    """Plot the paths in given JSON.

    Args:
        paths(list): A list of dictionaries that contain keys "name" to
        identify a path and "path" with the path data.
    """
    fig = plt.figure()
    for path in paths:
        style = "x-"
        if path["name"] == "original_path" or path["name"] == "agl_path":
            style = "o-"
        plt.plot(path["path"], style, label=path["name"])
    plt.legend()
    plt.show()

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-d",
        "--data",
        help="The output data to plot",
        type=str,
        required=True
    )
    args = parser.parse_args()

    with open(args.data) as f:
        paths = json.load(f)

    plot_paths(paths)
