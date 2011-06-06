#!/usr/bin/env python
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

"""
collect.py

This script builds tests data for all of the sample plans and genomes
"""
import os
import xml.dom.minidom
import random
from stylus import execute_stylus_plan
from data import BLACKLIST
from util import drop_extension

def find_genes():
    """
    Look through the sample directories finding all the genes
    """
    for path, directories, filenames in os.walk('sample'):
        for filename in filenames:
            if filename.endswith('.gene'):
                yield os.path.join(path, filename)

def find_plans():
    for filename in os.listdir('sample/plans'):
        if filename.endswith('.xml'):
            yield filename

def add_seed(dom):
    """
    Add a random seed to the genome xml dom
    """
    seed = dom.createElement("seed")
    seed.setAttribute("processorID", "4AFBBE2E-00CD-40B6-8A81-816F51B31EF6")
    seed_text = dom.createTextNode("%d %d" % (random.getrandbits(31), random.getrandbits(31) ) )
    seed.appendChild(seed_text)
    dom.documentElement.insertBefore(seed, dom.documentElement.firstChild)

def open_create_folders(path, mode):
    try:
        os.makedirs( os.path.dirname(path) )
    except OSError:
        # if the folder already exists thats okay
        pass
    return open(path, mode)

def process_gene(gene):
    dom = xml.dom.minidom.parse(gene)
    # to make sure behavior is consistent we add a seed to genome xml file
    add_seed(dom)

    # the genes are placed in folders based on rounding
    # to the nearest thousand
    holding_folder = os.path.basename(gene)[0] + '000'
    target_location = os.path.join('test_data', holding_folder, os.path.basename(gene) )
    target_file = open_create_folders(target_location, 'w')

    # dump that xml
    dom.documentElement.writexml(target_file)


def collect_test_data(plan, gene):
    plan_name = drop_extension(plan)
    gene_name = drop_extension(os.path.basename(gene))
    if (gene_name, plan_name) not in BLACKLIST:
        print "Collecting test data for plan: ", plan_name, "gene: ", gene_name
        target_path = os.path.join('test_data', plan_name)
        execute_stylus_plan(os.path.basename(gene), plan, './test_data', './sample/', 
                './sample/', './sample/plans', target_path)


def main():
    print "Processing gene files"
    genes = list(find_genes())

    for gene in genes:
        process_gene(gene)

    plans = list(find_plans())

    for plan in plans:
        for gene in genes:
            collect_test_data(plan, gene)

if __name__ == '__main__':
    main()
