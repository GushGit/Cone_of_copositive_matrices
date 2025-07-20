import subprocess
import matplotlib as plt

b_file_path = "../build/measurement_interface"

test_files = [
"gaussian_random_matrix_0_100_3_0_1_1_1_1",
"gaussian_random_matrix_0_100_3_30_0_0_0_0",
"border_cop_matrix_0_20_1_0_0_0_0_0",
"border_cop_matrix_0_20_3_15_1_1_1_1",
"gaussian_random_matrix_0_100_3_30_1_1_1_1",
"random_edge_1_1_3_0_1_1_1_1",
"k_regular_graph_1_1_3_0_1_1_1_1",
"k_regular_graph_1_1_3_3_1_1_1_1",
"k_regular_graph_1_4_3_10_1_1_1_1",
]

for tf in test_files:
  configuration = subprocess.run([b_file_path], input=tf,  capture_output=True, text=True)
  print(configuration.stdout)
  print(configuration.stderr)
