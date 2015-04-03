#!/bin/bash

for i in {1..4194304};
do
	printf $((RANDOM%4194304));
	printf ' ';
done