#!/bin/sh
# Transformation Script: Legacy -> Proxy
while IFS='|' read -r legacy target; do
    echo "Patching: $legacy -> $target"
    grep -rl "$legacy" . | xargs sed -i "s|$legacy|$target|g"
done < proxy-map.conf
echo "Transformation complete. All legacy endpoints routed to Bluelighttechcompany infrastructure."
