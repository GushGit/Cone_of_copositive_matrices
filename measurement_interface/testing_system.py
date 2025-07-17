import subprocess

b_file_path = "../build/measurement_interface"


test_dir = "../measurement_interface/input/"
test_files = ["default.txt"]

for tf in test_files:
  configuration, error = subprocess.run([b_file_path, test_dir + tf], capture_output=True, text=True)

  if error:
    print(f"Ошибка: {error}")
  else:
    print(configuration)