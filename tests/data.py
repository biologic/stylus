# The following is a list of gene-plan combinations which should
# not be run
BLACKLIST = [
    ('8C58', 'performance'), # performance.xml make specific references to 52DC
    ('7DDA', 'performance') # performance.xml make specific references to 52DC
]

IGNORE = {
        'history' : ['uuid', 'creationTool', 'creationDate'],
        'genome' : ['uuid', 'creationTool', 'creationDate'],
# the following two ignored because they contain line numbers
        'attempt' : ['description'],
        'compared' : ['description']
}
