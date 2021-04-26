import os
import shutil
import subprocess

root = os.getcwd()
tests_folder = root + "/mito/tests/"
if not os.path.isdir(tests_folder):
    raise NameError("Please run the script from the root dir")

# List of all folders
folders = [x for x in os.listdir(
    tests_folder) if os.path.isdir(tests_folder + x)]

output_file = tests_folder + 'out.log'

with open(output_file, 'w') as f:
    for folder in folders:
        folder_path = tests_folder + folder + "/"
        os.chdir(folder_path)
        test_name = "main"
        test_path = folder_path + test_name
        test_ext = ".cc"

        # Temporary folder for compile and run the test
        tmp_folder_path = folder_path + "temp_build/"
        if os.path.isdir(tmp_folder_path):
            shutil.rmtree(tmp_folder_path)
        os.mkdir(tmp_folder_path)

        # Commands to execute
        compile_cmd = 'g++ -g -std=c++2a -Wall -Wextra -pedantic -Werror ' + \
            test_path + test_ext + ' -o ' + tmp_folder_path + \
            test_name
        run_cmd = tmp_folder_path + test_name

        compile_process = subprocess.run(
            compile_cmd.split(' '), capture_output=True, text=True)
        if compile_process.stderr:
            f.write(folder + ": FAIL\n")
            f.write("Compilation error: \n" + compile_process.stderr)
            shutil.rmtree(tmp_folder_path)
            f.write("\n")
            continue

        run_process = subprocess.run(
            run_cmd.split(' '), capture_output=True, text=True)
        if run_process.stderr:
            f.write(folder + ": FAIL\n")
            f.write("Runtime error: \n" + run_process.stderr)
            shutil.rmtree(tmp_folder_path)
            f.write("\n")
            continue

        f.write(folder + ": SUCCESS\n")
        f.write("\n")
        shutil.rmtree(tmp_folder_path)

print("all done!")
