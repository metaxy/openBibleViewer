require 'open-uri'
require 'nokogiri'
require 'iso-639'
$base = "http://sourceforge.net"
$visited = Array.new()
class Link
    attr_accessor :name, :link, :lang;
end
  
def crawl(top, topname, res, start)
    
    if(top == nil || $visited.index(top) != nil)
        return
    end
    $visited.push(top);
    doc = Nokogiri::HTML(open(URI.parse(top)))
    links = (doc/'a[href]').map do |a| 
        link = a['href'];
        if(!(link.end_with?("stats/timeline") || (!link.start_with?(start) && !link.start_with?($base))))
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
            lang[$base+start] = "";
            x.link = $base+start+x.link
            x.lang = lang.split("/")[0];
            x.name = topname;
            res.push(x)
        else
            crawl($base + x.link, x.name, res, start) if not x.link.start_with?("http")
        end
    end
    return res
end
def main
  
   
    
    
    File.open("modules.xml", "w") do |x|
        x.puts '<?xml version="1.0" encoding="UTF-8"?>';
        x.puts '<modules version="0.1">';
        x.puts '<id type="bible">'
        bibles = crawl($base+"/projects/zefania-sharp/files/Bibles/", nil, Array.new(), "/projects/zefania-sharp/files/Bibles/")
        bibles.sort_by! {|x| x.lang }
       
        bibles.each do |y|
            lang = ISO_639.find_by_code(y.lang.downcase)
            if(lang == nil)
              lang = y.lang
            else
              lang = lang.english_name
            end
            x.puts "<item link=\"#{y.link}\" lang=\"#{lang}\">#{y.name}</item>";
        end
        
        x.puts ' </id>';
        
        x.puts '<id type="additional">'
        dict = crawl($base+"/projects/zefania-sharp/files/Dictionaries/", nil, Array.new(), "/projects/zefania-sharp/files/Dictionaries/")
        dict.sort_by! {|x| x.lang }
       
        dict.each do |y|
            lang = ISO_639.find_by_code(y.lang.downcase)
            if(lang == nil)
              lang = y.lang
            else
              lang = lang.english_name
            end
            x.puts "<item link=\"#{y.link}\" lang=\"#{lang}\">#{y.name}</item>";
        end
         x.puts ' </id>';
        x.puts '</modules>';
    end
end
main();
