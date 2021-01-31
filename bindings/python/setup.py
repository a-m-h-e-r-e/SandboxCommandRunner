import setuptools

VERSION = '0.0.1'
AUTHOR = 'Amir Kheiru'
AUTHOR_EMAIL = 'heiruamir@gmail.com'
DESCRIPTION = "Sandbox Command Runner for online judge"
LONG_DESCRIPTION = "Python package excuting and running program file"

# with open("README.md", "r", encoding="utf-8") as fh:
#     long_description = fh.read()

setuptools.setup(
    name="SandboxCommandRunner", # Replace with your own username
    version=VERSION,
    author=AUTHOR,
    author_email=AUTHOR_EMAIL,
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/markdown",
    url="https://github.com/pypa/sampleproject",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)