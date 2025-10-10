@echo off
(
echo rectangle red 500 500 400 400
echo triangle red 450 500 950 500 700 250 
echo regular-polygon yellow 10 250 50 50
echo.
) > input.txt
Designer.exe < input.txt
del input.txt