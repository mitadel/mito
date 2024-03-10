#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

import xml.etree.ElementTree as ET

# Load the XML file
tree = ET.parse('./memcheck.xml')

# Get the root element
root = tree.getroot()

# Check for elements named "error"
error_elements = [element for element in root.iter() if element.tag == "error"]

# Print the names of the error elements
if error_elements:
    raise RuntimeError("Errors found:", len(error_elements), "Please check valgrind output")
else:
    print("No errors found.")
