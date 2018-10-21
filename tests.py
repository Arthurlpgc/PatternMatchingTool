import os

pitches = "977SJLJLGJLJEVJLJL6JLJ4OJLJL@JLJ6QJLJLBJLJ@VJLJL4JLJN2JLJLBJLJ4OJLJLCJLJBSJLJL7JLJQ9IJIJ@IJI2NJLJLBJ"
sources = "art = (0xFFFF - (mc_size) + 1 ) ; /* Looks strange but ensures compiler only uses 16 bit unsigned in"
dblpxml = "The Design and Development of Resource-Sharing Services in Computer Communications Networks: A Surve"
dna = "GATCAATGAGGTGGACACCAGAGGCGGGGACTTGTAAATAACACTGGGCTGTAGGAGTGATGGGGTTCACCTCTAATTCTAAGATGGCTAGATAATGCAT"
proteins = "MGKSEGPVGMVESAGRAGQKRPGFLEGGLLLLLLLVTAALVALGVLYADRRGKQLPRLASRLCFLQEERTFVKRKPRGIPEAQEVSEVCTTPGCVIAAAR"
english = "of the intellectuals Lenin and Trotzky stood for insurrection. Even   urrection. Even   |          |"

path = "data/"
file = "english"
pattern = english

for algo in ["ShiftOr", "WuManber", "Ukkonen"]:
	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
		for size in range(bgn, end, stp):
			print("- - - - - - - - - - - ")
			print(file + " - " + algo + " - " + str(size))
			os.system("time pmt -c -a " + algo + " \"" + pattern[0:size] + "\" " + path + file)
for cmd in ["grep", "ggrep"]:
	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
		for size in range(bgn, end, stp):
			print("- - - - - - - - - - - ")
			print(file + " - " + cmd + " - " + str(size))
			os.system("time " + cmd + " -c \"" + pattern[0:size] + "\" " + path + file)