Disk partition on Windows

I always forget which tool I should use to partitioning external disk in a not so common way.
I noted down here. I hope I don't forget next time.

## Physically set zero all sector in disk
1. Install Git for Windows
2. Insert USB disk
3. Remove all other than internal disk (don't forget)
3. Launch Git Bash with Administrator account
4. Check under /dev to check which dev file is the external one
(if you don't confident, play with it, create partition , delete)
5. Once you know which disk (usually /dev/sdb ) type command:
```
dd if=/dev/zero of=/dev/sdb bs=1M
```

## Convert MBR (or create MBR with Windows compatible)
1. Run Cmd.exe with Administrator
2. Type 'diskpart'
3. List disk
4. Select the external disk
5. Convert to MBR
6. Create a primary partition

## Convert GPT (or create GPT with Windows compatible)
1. Run Cmd.exe with Administrator
2. Type 'diskpart'
3. List disk
4. Select the external disk
5. Convert GPT
6. Create a partition

> Note: Still many embedded systems ( e.g. office printers ) don't recognize GPT partition disk, if the disk will be used non-PC or very old PC, the safest bet is Windows compatible MBR partition.

> Note2: I want to copy diskpart behavior if feasible... at least when dictate to create one partition less than 1TB disk (this is only possible either reverse engineering or tracing WinAPI and check dump partition disk raw in each command)


### TODO: How about Mac?