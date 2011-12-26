require 'open-uri'
require 'nokogiri'

$base = "http://sourceforge.net"
$start = "/projects/zefania-sharp/files/Zefania%20XML%20Modules%20%28new%29/"

$end = Array.new()
$visited = Array.new()
class Link
    attr_accessor :name, :link, :lang;
end
def crawl(top, topname)
    
    if(top == nil || $visited.index(top) != nil)
        return
    end
    puts "top = #{top}"
    $visited.push(top);
    
    doc = Nokogiri::HTML(open(URI.parse(top)))
    links = (doc/'a[href]').map do |a| 
        link = a['href'];
        if(!(link.end_with?("stats/timeline") || (!link.start_with?($start) && !link.start_with?($base))))
            l = Link.new();
            l.link = link;
            l.name = a.children[0].content.strip;
            l
        else
            nil
        end
    end
    links.delete_if {|x| x == nil}
    links.each do |x|
        if(x.link.end_with?(".zip/download"))
            x.link["/download"] = "";
            lang = x.link;
            lang[$base+$start] = "";
            x.link = $base+$start+x.link
            x.lang = lang.split("/")[0];
            x.name = topname;
            $end.push(x)
        else
            crawl($base + x.link, x.name)
        end
          
    end
end
def main
    crawl($base+$start, "")
    File.open("modules.xml", "w") do |x|
        x.puts '<?xml version="1.0" encoding="UTF-8"?>';
        x.puts '<MODULES version="0.1">';
        x.puts '<ID type="bible">'
        $end.sort_by! {|x| x.lang }
        $end.each do |y|
            x.puts "<ITEM link=\"#{y.link}\" lang=\"#{y.lang}\">#{y.name}</ITEM>";
        end
        x.puts ' </ID>';
        x.puts '</MODULES>';
    end
end
main();