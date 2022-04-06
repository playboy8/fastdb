import sys
import os
sys.path.append(os.path.realpath('.'))
import cli_py
import numpy as np



cli = cli_py.cli_python()
print(cli)
print('---------------')
cli.cli_python_init('127.0.0.1:6100','testpar', '', 100*1024*1024,100*1024*1024,100*1024*1024, 0)
rc = cli.open(10,1)
print(rc)
print('---------------')

