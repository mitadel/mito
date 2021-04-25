import os
import shutil

# List of all folders
root = os.getcwd() + "/mito/tests/"
folders = [x for x in os.listdir(root) if os.path.isdir(root + x)]

output_file = root + 'out.txt'

with open(output_file, 'w') as f:
    for folder in folders:
        folder_path = root + folder + "/"
        test_name = "main"
        test_path = folder_path + test_name
        test_ext = ".cc"

        tmp_folder_path = folder_path + "temp_build/"
        os.mkdir(tmp_folder_path)

        compile_cmd = "g++ -g -std=c++2a -Wall -Wextra -pedantic -Werror " + \
            test_path + test_ext + " -o " + tmp_folder_path + test_name
        run_cmd = tmp_folder_path + test_name

        if os.system(compile_cmd) == 0:
            if os.system(run_cmd) == 0:
                f.write(folder + ":\tSuccess")
                print("Done")
        else:
            f.write(folder + ":\tFailed")
            print("Failed")
        shutil.rmtree(tmp_folder_path)
        f.write("\n")

print("all done!")
