import os

pitches = "977SJLJLGJLJEVJLJL6JLJ4OJLJL@JLJ6QJLJLBJLJ@VJLJL4JLJN2JLJLBJLJ4OJLJLCJLJBSJLJL7JLJQ9IJIJ@IJI2NJLJLBJ"
sources = "art = (0xFFFF - (mc_size) + 1 ) ; /* Looks strange but ensures compiler only uses 16 bit unsigned in"
dblpxml = "The Design and Development of Resource-Sharing Services in Computer Communications Networks: A Surve"
dna = "GATCAATGAGGTGGACACCAGAGGCGGGGACTTGTAAATAACACTGGGCTGTAGGAGTGATGGGGTTCACCTCTAATTCTAAGATGGCTAGATAATGCAT"
proteins = "MGKSEGPVGMVESAGRAGQKRPGFLEGGLLLLLLLVTAALVALGVLYADRRGKQLPRLASRLCFLQEERTFVKRKPRGIPEAQEVSEVCTTPGCVIAAAR"
english = "of the intellectuals Lenin and Trotzky stood for insurrection. Even   urrection. Even   |          |"

path = "data/"
file = "sources"
pattern = sources


# TESTE DE PADRÃO DE TAMANHOS VARIÁVEIS
# for algo in ["ShiftOr", "WuManber", "Ukkonen", "AhoCorasik"]:
# 	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
# 		for size in range(bgn, end, stp):
# 			print("- - - - - - - - - - - ")
# 			print(file + " - " + algo + " - " + str(size))
# 			os.system("time pmt -c -a " + algo + " \"" + pattern[0:size] + "\" " + path + file)
# for cmd in ["grep", "ggrep"]:
# 	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)]:
# 		for size in range(bgn, end, stp):
# 			print("- - - - - - - - - - - ")
# 			print(file + " - " + cmd + " - " + str(size))
# 			os.system("time " + cmd + " -c \"" + pattern[0:size] + "\" " + path + file)

# TESTE COM MULTIPLOS PADRÕES EM UM ARQUIVO
# with open('pattern-' + file + '.txt', 'w') as fl:
# 	for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)][::-1]:
# 		for size in list(range(bgn, end, stp))[::-1]:
# 			fl.write(pattern[0:size] + '\n')
# 
# for algo in ["ShiftOr", "WuManber", "Ukkonen", "AhoCorasik"]:
# 	for i in range(1, 24):
# 		with open('pattern-' + file + '.txt', 'w') as fl:
# 			count = 0;
# 			for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)][::-1]:
# 				for size in list(range(bgn, end, stp))[::-1]:
# 					if count < i:
# 						fl.write(pattern[0:size] + '\n')
# 						count += 1
# 					else:
# 						break
# 		print("- - - - - - - - - - - ")
# 		print(file + " - " + algo + " - " + str(i))
# 		os.system("time pmt -c -a " + algo + " -p pattern-" + file + ".txt " + path + file)
# for cmd in ["grep", "ggrep"]:
# 	for i in range(1, 24):
# 		with open('pattern-' + file + '.txt', 'w') as fl:
# 			count = 0;
# 			for bgn, end, stp in [(1, 10, 1), (10, 50, 5), (50, 101, 10)][::-1]:
# 				for size in list(range(bgn, end, stp))[::-1]:
# 					if count <= i:
# 						fl.write(pattern[0:size] + '\n')
# 						count += 1
# 					else:
# 						break
# 		print("- - - - - - - - - - - ")
# 		print(file + " - " + cmd + " - " + str(i))
# 		os.system("time " + cmd + " -c -f pattern-" + file + ".txt " + path + file)

pitches = "977SJLJLGJLJEVJLJ"
sources = "art = (0xFFFF - ("
dblpxml = "The Design and De"
dna = "GATCAATGAGGTGGACA" 
proteins = "MGKSEGPVGMVESAGRA" 
english = "of the intellectu"

path = "data/"
file = "pitches"
pattern = pitches

for algo in ["WuManber", "Ukkonen"]:
	for edit in range(0, 16):
			print("- - - - - - - - - - - ")
			print(file + " - " + algo + " - " + str(edit))
			os.system("time pmt -c -a " + algo + " -e " + str(edit) + " \"" + pattern + "\" " + path + file)
for cmd in ["agrep"]:
	for edit in range(0, 16):
			print("- - - - - - - - - - - ")
			print(file + " - " + cmd + " - " + str(edit))
			os.system("time " + cmd + " -c  -" + str(edit) + " \"" + pattern + "\" " + path + file)



