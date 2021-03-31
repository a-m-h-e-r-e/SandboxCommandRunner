import setuptools

VERSION = '0.0.1'
AUTHOR = 'Amir Kheiru'
AUTHOR_EMAIL = 'heiruamir@gmail.com'
DESCRIPTION = "Sandbox Command Runner for online judge"
LONG_DESCRIPTION = "Python package excuting and running program file"



setuptools.setup(
    name="SandboxCommandRunner",
    version=VERSION,
    author=AUTHOR,
    author_email=AUTHOR_EMAIL,
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/markdown",
    url="https://github.com/SJ-Kheiru/SandboxCommandRunner",
    packages=setuptools.find_packages(),
    python_requires='>=3.6',
)
