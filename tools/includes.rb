require 'find'
require 'fileutils'
require 'colorize'

$src_path = '../src'
$whitelist = ['"config.h"', '"src/core/dbghelper.h"']
$blacklist = [$src_path+"/extern"]
$c_file_suffix = '.cpp'
$h_file_suffix = '.h'

def find_includes(text)
	includes = Array.new
	ifdef_level = 0
	text.each_line do |x|
		x.strip!
		ifdef_level += 1 if x.start_with?('#ifdef')
		ifdef_level -= 1 if x.start_with?('#endif')

		if(x.start_with?('#include') and ifdef_level == 0)
			x['#include '] = ''
			
			includes.push(x) unless $whitelist.include?(x)
		end
	end
	return includes
end

def remove_include(file, inc)
	f = File.new(file, "r")
	text = String.new()
	while(line = f.gets)
		# just don't write the include
		next if line.start_with?('#include ' + inc)
		text << line
	end
	File.open(file, 'w') { |f| f.write(text) }

end

def test_compile()
	system('cd ../build && make')
end

def make_backup(file)
	FileUtils.cp(file, file + ".back")
end

def rm_backup(file)
	begin
		FileUtils.rm(file + ".back")
	rescue
	end

end

def restore_backup(file)
	FileUtils.rm(file)
	FileUtils.mv(file+".back", file)
end

puts test_compile()

def rm_inc(f, i)
	puts "----- #{f} trying to remove #{i}".yellow
	make_backup(f)
	remove_include(f,i)
	if(not test_compile())
		restore_backup(f)
	else
		puts "succesfully remove #{i} in #{f}".green
	end
	rm_backup(f)

end
def main()
	Find.find($src_path) do |f|
		skip = false
		$blacklist.each {|x| skip = true if f.start_with?(x)}
		next if skip
		next unless f.end_with?($c_file_suffix)

		h_file = String.new(f)
	
		h_file[$c_file_suffix] = String.new($h_file_suffix)

	
		next unless File.exists?(h_file)

		begin
			if(File.read('files_includes_ok.txt').include?(f))
				puts "skipping file #{f} because it is ok"
				next
			end
		rescue
		end

		cpp = File.read(f);
		hpp = File.read(h_file)
		
		cpp_inc = find_includes(cpp)
		hpp_inc = find_includes(hpp)

		includes_to_remove = Array.new
		inter = cpp_inc & hpp_inc
		
		if(inter != [])
			puts "found intersection in " + f
			includes_to_remove.push(inter)
		end
		
		cpp_inc.each do |i|
			next if includes_to_remove.include?(i)
			rm_inc(f,i)		
			sleep 1
		end
		hpp_inc.each do |i|
			next if includes_to_remove.include?(i)
			rm_inc(h_file,i)		
			sleep 1
		end
		includes_to_remove.flatten!

		includes_to_remove.each do |i|
			rm_inc(f,i)
			sleep 2
		end
		puts "######### commit"
		if(test_compile())
			system "git add #{h_file} #{f}"
			system "git commit -m'remove includes in #{f}'"
		else
			system "git reset #{f} #{h_file}"
		end
		sleep 2

		File.open('files_includes_ok.txt', 'a') { |a|
			  a.puts "#{f}"
		}
	end
end

# run
main()
