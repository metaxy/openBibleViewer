require 'nokogiri'

def main()
    Dir.foreach("../modules").each do |x|
        next if x == "." or x == ".."
        doc = Nokogiri.XML(File.read("../modules/"+x))
        name = doc.xpath('//meta/name')[0]
        puts "<ITEM link=\"http://metaxy.github.com/obv/modules/#{x}\" lang=\"English\" size = \"0.1 MB\">#{name}</ITEM>";
    end
end
main()
