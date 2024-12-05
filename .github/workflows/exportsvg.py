import base64
import sys

with open("graph.svg", "rb") as f:
    encoded_svg = base64.b64encode(f.read()).decode("utf-8")

filename = sys.argv[1]
with open(filename, 'w') as f:
    f.write(f"![svg data](data:image/svg+xml;base64,{encoded_svg})")
