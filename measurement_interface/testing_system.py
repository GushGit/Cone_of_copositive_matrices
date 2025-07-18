import subprocess
import matplotlib.pyplot as plt

b_file_path = "../build/measurement_interface"


test_dir = "../measurement_interface/input/"
test_files = [
  "k_regular_graph_1_4_3_3_1_1_1_1",

]

for tf in test_files:
  configuration = subprocess.run([b_file_path], input=test_dir + tf,  capture_output=True, text=True)
  print(configuration.stdout)
  print(configuration.stderr)
