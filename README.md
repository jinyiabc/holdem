# holdem

http://www.archduke.org/simplex/index.html
http://jean-pierre.moreau.pagesperso-orange.fr/c_linear.html

Chen Bill's Jam/Fold table introduce a hand range for attacker/defender deciding
in which stack size the player should start to jam(all-in) or fold by restricting player's actions to jam and fold.
By a little modification to a nice tool by A.P.Selby 1999, we can reverse-engineer which hand range support jam or fold
action given a specified stack size by raise argument. The results are exactly matched to Bill's table.

Compile using the command:
gcc -o sim sim_jam_fold.c -lm -O3

Run the command using:
sim an=0.5 bl=0.5 lim=1 r=<stack size> liv=0\
 d0=169 d1=169 nr=1 pl=0 pr=6

Interesting results: A.P.Selby's linear programming objective function maximize sum of m(k) in line 137(sim.c).
That means a hand range allowed under specified stack size is essential compared to single hand under Nash's equilibrium theory.
