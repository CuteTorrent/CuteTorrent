import string

# Open a file
fo = open("core/Build_Increment.h", "r+")
print("Name of the file: ", fo.name)
line = fo.readlines()
build = 0;
revision_increment = 0;
for idx, item in enumerate(line):
	if 'BUILD_INCREMENT' in item:
		tmp_str =  item.split();
		build = int(tmp_str[2])+1;
		if build > 100:
			revision_increment = 1;
			build = 0;
		tmp_str[2] = str(build)+"\n";
		item = ' '.join(tmp_str);
		print(item);
		line[idx] = item;
	if 'REVISION_INCREMENT' in item:
		tmp_str =  item.split();
		tmp_str[2] = str(int(tmp_str[2]) + revision_increment)+"\n";
		item = ' '.join(tmp_str);
		line[idx] = item;
		print(item);
fo.seek(0)

for idx, item in enumerate(line):
    fo.write(item)
fo.truncate()

# Close opend file
fo.close()