
import glob, os, sys

# Loading too many will cause failures on the browser.
MAX = -1

if len(sys.argv) > 1 and '--max' == sys.argv[1]:
    MAX = int(sys.argv[2])

output = "var snapshots = {\n"

num_files = len(glob.glob("fond-snapshot.*.out"))
for i in range(num_files):

    fname = "fond-snapshot.%d.out" % i

    if MAX == 0:
        break
    MAX -= 1

    output += "%s: {\n" % fname.split('.')[1]
    with open(fname) as f:
        output += f.read()
    output += "},\n"
output += "};"

with open(os.path.join(os.path.dirname(__file__), "js", "snapshots.js"), 'w') as f:
    f.write(output)

os.system('rm fond-snapshot.*.out')
