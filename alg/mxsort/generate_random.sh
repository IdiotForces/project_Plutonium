#!/bin/bash

for i in {1..128};
do
	printf $((RANDOM%256));
	printf ', ';
done