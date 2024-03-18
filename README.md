# F28HS Coursework 1

## My task
I found my task in task list.
> H00391678: HSHEX NOISE COMP

## How to run my program
Firstly, use `make` to compile my program, since a Makefile is provided.<br>
Secondly, run it from the command-line. Use arguments like below.<br>
Without the Bonus task, the arguments should like this.<br>
> `./process reference.image input.image output.image noise_strength`  

After adding the Bonus task, the arguments should like this.<br>
> `./process img_num reference.image input.image1 input.image2 ... input.imageN output.image1 output.image2 ... output.imageN noise_strength`

## Question 1 and 2
I didn't add any parameters.

## Question 3(NOISE)
My NOISE function process the `input.image`, and save the processed image in `output.image`.<br>
Also, a `noise_strength` parameter is added in the end.

## Question 4(COMP)
My COMP function compare the `reference.image` with each `input.image`.

## Question 5(Bonus task)
`img_num` parameter is added to input number of images.<br>
`input.image1` to `input.imageN` means input image filenames.<br>
`output.image1` to `output.imageN` means input image filenames.<br>
