# 394-Shutter-Switch

This is the location for the software associated with the shutter switch for job 394.

Description of Function

The idea is that when a 5v signal is recieved, the relay energises, pushing the switch to the left or right.
But, once the switch as reached the end of its travel, continuing to allow current to flow is pointless and results in unnesicary heating.
Therefore the switch is desigined to deenergise after a predetermined amount of time, typically 0.25 seconds.
If the 5v signal is removed, the software detects that change, and reenergises the coil to puch it back to the initial position,
again deenergizing after a prededtermined amount of time.


