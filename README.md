# random-csv-generator
A command line utility for generating CSV files with random data, for the purposes of testing d3 visualizations.

Use it like this:

random-csv-generator filename column1 ... #rows seed
- filename and #rows are self explanatory
- seed is the seed for the random number generator
- columns are entered in this format:

type header
- header is the name of the data series
- type is one of f, i, b, s, or p, and has the following behavior:

f - fills the column with floating-point numbers
i - fills the column with integers
p - fills the column with (floating-point) percentages between 0 and 100
b - fills the column with the string representation of TRUE/FALSE values
s - fills the column with strings with a length from 5 to 10

As an example, this command:
```
random-csv-generator.exe out i "Participant ID" s Name b "Participant Paid" f "Amount Paid" b "Response Received" p Response 100 1337
```
generates this output:

![csv](out.csv)
