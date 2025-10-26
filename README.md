# miniOdy
yep.. This is our minihell's odyssey. Almost as if Ody hadn't gone through enough :|

### How long has it been? 20 years....

Starting from July 28th, after a lot of trial and errors and moral support spamming, we've done it. On the 26th of October.
Introducing, the not-so-original, minishell. Nicknamed miniOdy for funzies and cs we like EPIC: The Musical :))
Functionally no different from any other minishell as this is just a literal coughing baby made to work like the atomic bomb that is bash
But spiritually, its our miniOdy :333 just look at the soulful lil references in the project hehe

### @leensal did Parsing and I @normcrusher42 did execution and a small drop of parsing

## First things first, credits <3

My favorite part about working on my projects are the people that helped make an influence or contributed to testing/helping the project along the way. Really making a mark on this journey of ours <3
That goes for moral aid too hehe. I'm saving a certain somebody for last that's deserved it.

From the order of which we recieved support, I'd first like to thank our boy @Basheer for helping us almost fully troubleshoot a mysterious leak (I found the small culprit)

A couple of my 42 friends, including @tabadawi @ashalaab, @ehammoud, @Pastifier & @ahasna for checking the validity of our project and the functions its used as some things weren't clear enough ^^'

especially HUGE thanks to @ehammoud for testing our minishell before submission, pinpointing a very specific issue that used to not be present!!! XoX 
And for really useful feedback that helped make our minishell better than it ever was! (and a lil bit of testing from @ashalaab too that helped a bit heh)

and ofc the comparisons that I did with my friends' minishells (specifically @Pastifier, @iMan, and @ZeroQL) that helped me confirm and debug things and take references from their proejct.
Saw how they handled/mishandled some issues and patched our minishell accordingly 👉👈 their minishells are still incredible tho and done in a much shorter time than ours

and ofc @Riya for being one of my moral supports again during another one of my hectic projects LOL she was even there to hear me suffer in Deepnest in HK HAHAHA....ha..

if I forget anyone please forgive me as this project had me calling Qtips "ear wax" 😭

saving one more special person at the end so KEEP READING

## How it works

Too lazii to explain. Parsing was done by @leensal and all I know is we're using the ASL parser (Absolutely - Sane - Leen)
as opposed to an AST parser so bug her for the description if you're that curious LOL.

I don't think much explaining is needed for execution, I just did le builtins, path exec logic, and env. They work literally as they do in Bash.

## How to run

It's fairly simple. And there's 2 ways to do it:

### Automatically Compile and Run at once

1. run `Make runngun`, wait a bit, and voila ✨ project compiled and running

### Compile and Run once at a time

1. run `Make` in the root directory of the project and wait for the compilation to complete with two ✅.
2. run or bash `./minishell` 

and there you have it :)))

## How to run with valgrind

This is even simpler than the first as I've already included suppression files for readline and /bin/ functions:

1. run `Make leak` and it will compile and auto run the program with valgrind and its flags

(you may copy the flags manually from the makefile if you need to)

## Easter Eggs

Yes! Those exist! Although not much of it just to be safe... but if you're an EPIC: The Musical fan, you'd get them hehe
I'll list how to find them here under a spoilers flag (there is no spoiler flag... READ AWAY):

try expanding "$$"
crash any program (try executing code that dereferences a null pointer ;3)
huh.. I thought I included more easter eggs.. oh well.

some records of our journey is available in the "txt junk" folder. Constantly changing roadmaps, a checklist, etc.. its worth the look lool



and of course before I forget,

# An honorable thank you message to my project partner and 42 sister, Beans (aka @leensal)

I know how tough its been for you to deal with work-life and minishell and I tried doing everything in my power
to make things easier for you, even if it means stalling minishell for 3 months... 💀 we better not get blackholed for this.
But regardless, I'm so very proud of the work you were able to pull off in spite of all this and you did your best to not leave
me hanging several times and I cannot appreciate you enough for that >o< like the fun moments we had and our shared support for each
other was what made working on this project so much more enjoyable I couldn't have asked for a better minishell partner during
the spot I was in at the time.

I say this with so much appreciation from your little 42 brother idk what to say, thank you sooo soo much for being my minishell partner <3
Imma literally ctrl + C ctrl + V your line cs I can, I feel so blessed to work with you on this project hehehehe
Your efforts helped us create an Epic minishell program and made those 3 months worth the time it took for a project we just
wanted to move on from LOL
Stay diligent Beanie, don't let anything ever make you feel doubtful about your work, you can do wonders with persistency and a sip of hope hehe

We're half way through our 42 journey but another memorable arc is about to begin at rank 4✨
we may not work together next rank (idk about rank 5) but I look forward to forming our transcendance team with you again :D

imma go to sleep now. Our minishell better have passed by the time you're reading this LMAO


p.s. dessert is still on me and you're NOT doing anything about it! my turn >:3


