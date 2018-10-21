import os

pitches = "977SJLJLGJLJEVJLJL6JLJ4OJLJL@JLJ6QJLJLBJLJ@VJLJL4JLJN2JLJLBJLJ4OJLJLCJLJBSJLJL7JLJQ9IJIJ@IJI2NJLJLBJ"
source = "art = (0xFFFF - (mc_size) + 1 ) ; /* Looks strange but ensures compiler only uses 16 bit unsigned in"
dna = "GATCAATGAGGTGGACACCAGAGGCGGGGACTTGTAAATAACACTGGGCTGTAGGAGTGATGGGGTTCACCTCTAATTCTAAGATGGCTAGATAATGCAT"
english = "of the intellectuals Lenin and Trotzky stood for insurrection. Even   urrection. Even   |          |"
proteins = "MGKSEGPVGMVESAGRAGQKRPGFLEGGLLLLLLLVTAALVALGVLYADRRGKQLPRLASRLCFLQEERTFVKRKPRGIPEAQEVSEVCTTPGCVIAAAR"
dblpxml = "The Design and Development of Resource-Sharing Services in Computer Communications Networks: A Surve"

path = "data/"
file = "pitches"
pattern = pitches

for algo in ["ShiftOr", "WuManber", "Ukkonen", "AhoCorasik"]:
	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
		for size in range(bgn, end, stp):
			print("- - - - - - - - - - - ")
			print(algo + " - " + str(size))
			os.system("time pmt -c -a " + algo + " \"" + pattern[0:size] + "\" " + path + file)
for cmd in ["grep", "ggrep"]:
	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
		for size in range(bgn, end, stp):
			print("- - - - - - - - - - - ")
			print(cmd + " - " + str(size))
			os.system("time " + cmd + " -c \"" + pattern[0:size] + "\" " + path + file)