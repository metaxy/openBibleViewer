@list =  process_files ("/home/paul/c++/openBibleViewer/src");
@cpp = grep { /\.cpp$/ } @list;
@h = grep { /\.h$/ } @list;
foreach $file (@cpp) {
    open (F, $file);
    $record = <F>;
    if(!(index ($record,"*") > -1) && !(index ($file,"clucene") > -1)) {
        print $file."\n";
    }
    close(F);
}
foreach $file (@h) {
    open (F, $file);
    $record = <F>;
    if(!(index ($record,"*") > -1) && !(index ($file,"clucene") > -1)) {
        print $file."\n";
    }
    close(F);
}
sub process_files {
    my $path = shift;
    opendir (DIR, $path)
        or die "Unable to open $path: $!";
    my @files =
        map { $path . '/' . $_ }
        grep { !/^\.{1,2}$/ }
        readdir (DIR);
    closedir (DIR);

    for (@files) {
        if (-d $_) {
            push @files, process_files ($_);
        } 
    }
    return @files;
}
