#!/bin/sh
rm -rf *.htm *.html
#sudo ifconfig ens33 down
bikeshed
#sudo ifconfig ens33 up
echo "Editing all links so that they open a new browser tab..."
sed -i -E 's/href=\"http([^\"]+)\"/href=\"http\1\" target="_blank"/g' *.html
echo "Changing \'Latest published version\' to \'Latest version\'..."
sed -i 's/Latest published version:/Latest version:/g' *.html
echo "Putting the email address explanation in italics..."
sed -i 's/Remove all sevens from email address/<i>Remove all sevens from email address<\/i>/g' *.html
