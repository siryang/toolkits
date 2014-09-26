/**
You are given a paragraph of text, all in one line. 
These snippets of text were extracted from articles about people and places, from Wikipedia. 
Certain letters (a-z or A-Z) have been blanked out and replaced by '#' signs instead. 
Identify what those letters originally were.

Input Format 
Exactly one line of a text snippet - which may contain either one sentence, 
or a paragraph (of multiple sentences). 
At certain positions, letters (a-z or A-Z) have been blanked out and replaced by '#' signs instead.

Constraints 
Number of characters in the input will not exceed 2000. 
There will be no extra '#' other than those which need to be replaced by letters. 
Please note that the text may have non-ASCII (UTF-8) characters. 
However, the the valid replacements for '#' symbols, in all cases, are one of the letters (a-z or A-Z; case agnostic). 
Characters have been blanked out with approximately 5% probability; 
i.e. roughly 1 in 20 letters are missing.

Output Format 
The output should contain as many lines as the number of '#' signs in the input text. 
The ith line should contain the letter of the alphabet which was originally present in place of the ith hash symbol. 
Evaluation will be case insensitive.

Sample Input

William Shakespea#e was an Englis# poet, playwright a#d actor, widely regarded as the greatest writer in the Eng#ish langu#ge and the worl#'s pre-eminent dramat#st. He is ofte# called Englan#'s national poet an# the "Bard of Avon".  
Sample Output

r
h
n
l
a
d
i
n
d
d  

Training File

You may download and use this corpus of text for offline training and constructing a model which may be 
embedded in your submitted solution. This will not be available during program execution.

Regular ML and NLP libraries, as described in the environment will be available.

Explanation

The first of the missing characters is the letter 'r' in Shakespea*r*e,
the second missing character is the 'h' in the word 'Englis*h*';
the third missing character is 'n' in the word 'a*n*d' - and so on.

The original snippet of text was: 
William Shakespeare was an English poet, playwright and actor, 
widely regarded as the greatest writer in the English language and the world's pre-eminent dramatist. 
He is often called England's national poet and the "Bard of Avon".

Scoring

For each test case your score will be: C/N 
Where C is the number of characters correctly identified; 
and N is the total number of characters which were blanked out and replaced by hash symbols (#). 
Your overall score is the average of your score across all test cases.
*/