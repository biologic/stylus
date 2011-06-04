import subprocess
import sys

class StylusExecutionError(Exception):
    pass
        

def execute_stylus_plan(gene, plan, genome_url, han_url, html_url, plan_url, data_dir):
    """
    Execute the stylus program, no output or return value.
    If stylus fails an StylusExecutionError exception will be raised
    """
    if '-d' in sys.argv:
        version = 'd'
    else:
        version = 'r'

    stylus = subprocess.Popen(['./stylus', version, '-e', '--', '-g', gene, '-p', plan,
        '-u', ','.join([genome_url, han_url, html_url, plan_url]), '-d', data_dir], 
        stdout = subprocess.PIPE, stderr = subprocess.STDOUT)

    stdout, stderr = stylus.communicate()
    if stylus.returncode != 0:
        raise StylusExecutionError(stdout)


if __name__ == '__main__':
    execute_stylus_plan('52DC.gene', 'simple.xml', './sample/', './sample/', './sample/', './sample/plans', 'alpha')
