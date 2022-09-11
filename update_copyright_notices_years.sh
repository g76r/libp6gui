#!/bin/bash
sed -E -i 's/(Copyright [12][0-9]{3}) /\1-2022 /;s/(Copyright [12][0-9]{3})-[0-9]{4} /\1-2022 /' $(grep -Erl 'Copyright.*[12][0-9]{3}(-[0-9]{4})?'|grep -E 'h|cpp|pro$')
