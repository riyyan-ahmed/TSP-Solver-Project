"""
A random instance generator for the TSP
"""
from typing import List, Tuple
from random import random
from os import mkdir, path
import argparse

def generate_coordinates(instance_size: int, min_x: float, max_x: float,
                          min_y:float, max_y:float) -> List[Tuple[float, float]]:
    """
    Generates a list of random coordinates
    
    :param instance_size: number of coordinates
    :type instance_size: int
    :param min_x: minimum value for x coordinate
    :type min_x: float
    :param max_x: maximum value for y coordinate
    :type max_x: float
    :param min_y: minimum value for x coordinate
    :type min_y: float
    :param max_y: maximum value for y coordinate
    :type max_y: float
    """
    coordinate_list = []
    for _ in range(instance_size):
        x = min_x + random() * (max_x - min_x)
        y = min_y + random() * (max_y - min_y)
        coordinate_list.append((round(x, 2), round(y, 2)))
    return coordinate_list

def write_instance(inst_name: str, filepath: str, 
                   coordinates: List[Tuple[float, float]]) -> None:
    """
    Write the instance in the TSP file format
    
    :param inst_name: instance name
    :type inst_name: str
    :param filepath: path to file
    :type filepath: str
    :param coordinates: list of coordinates
    :type coordinates: List[Tuple[float, float]]
    """
    with open(filepath, 'w', encoding="utf-8") as inst_file:
        inst_file.write(f"NAME : {inst_name}\n")
        inst_file.write("TYPE : TSP\n")
        inst_file.write(f"DIMENSION : {len(coordinates)}\n")
        inst_file.write("EDGE_WEIGHT_TYPE: EUC_2D\n")
        inst_file.write("NODE_COORD_SECTION\n")
        for index, coord in enumerate(coordinates):
            inst_file.write(f"{index} {coord[0]} {coord[1]}\n")
        inst_file.write("EOF")

def create_benchmark(folder_path: str, inst_prefix: str, nb_inst: int=100, instance_size: int=20,
                     min_x: float = 0, max_x: float = 1000, min_y:float = 0, max_y: float = 1000):
    """
    Creates a randomly generated benchmark of instances in the folder
    
    :param folder_path: path to the instance folder
    :type folder_path: str
    :param inst_prefix: name prefix for the instances
    :type inst_prefix: str
    :param nb_inst: number of instances to be generated
    :type nb_inst: int
    :param instance_size: size of the instances (#vertices)
    :type instance_size: int
    :param min_x: minimum value for x coordinate
    :type min_x: float
    :param max_x: maximum value for x coordinate
    :type max_x: float
    :param min_y: minimum value for y coordinate
    :type min_y: float
    :param max_y: maximum value for y coordinate
    :type max_y: float
    """
    try:
        mkdir(folder_path)
    except FileExistsError:
        pass
    for i in range(nb_inst):
        inst_name = f"{inst_prefix}-{i}.tsp"
        inst_path = path.join(folder_path, inst_name)
        with open(inst_path, 'w', encoding='utf-8'):
            coordinates = generate_coordinates(instance_size, min_x, max_x, min_y, max_y)
            write_instance(inst_name, inst_path, coordinates)

def main():
    """
    Main function to parse arguments and create benchmark
    """
    parser = argparse.ArgumentParser(description="Random TSP instance generator")
    parser.add_argument("folder_path", type=str, help="Path to the instance folder")
    parser.add_argument("inst_prefix", type=str, help="Name prefix for the instances")
    parser.add_argument("--nb_inst", type=int, default=100, help="Number of instances")
    parser.add_argument("--instance_size", type=int, default=20, 
                        help="Size of the instances (number of vertices)")
    parser.add_argument("--min_x", type=float, default=0, help="Minimum value for x coordinate")
    parser.add_argument("--max_x", type=float, default=1000, help="Maximum value for x coordinate")
    parser.add_argument("--min_y", type=float, default=0, help="Minimum value for y coordinate")
    parser.add_argument("--max_y", type=float, default=1000, help="Maximum value for y coordinate")
    args = parser.parse_args()
    create_benchmark(args.folder_path, args.inst_prefix, args.nb_inst,
                     args.instance_size, args.min_x, args.max_x,
                     args.min_y, args.max_y)

if __name__ == "__main__":
    main()
