import os.path

def drop_extension(path):
    return os.path.splitext(path)[0]
