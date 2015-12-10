# Stylus, Copyright 2011 Biologic Institute
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
