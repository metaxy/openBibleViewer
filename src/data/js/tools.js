function VerseSelection() {
	this.getSelection = verseGetSelection;
	this.startVerse = -1;
	this.endVerse = -1;
	this.repeat = 0;
	this.selectedText = "";
	this.shortesStringInStartVerse = "";
	this.shortesStringInEndVerse = "";
    this.startVerseText = "";
    this.endVerseText = "";
    
    this.moduleID = -1;
    this.bookID = -1;
    this.chapterID = -1;
}
function verseGetSelection () {
	if(window.getSelection().type == 'Range') {
 		var a = window.getSelection().getRangeAt(0);
        if(a != null) {
            var start = a.startContainer;
            var end = a.endContainer;
            var it = start;
            while(true) {
                var e = it.parentNode;
                if( e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    this.startVerse = e.getAttribute("verseID");
                    this.startVerseText = e.textContent;
                    break;
                } else {
                    it = e;
                }
            }
            
            it = end;
            while(true) {
                var e = it.parentNode;
                if(e == null)
                    break;
                if(e.getAttribute("verseID") != null) {
                    this.endVerse = e.getAttribute("verseID");
                    this.moduleID = e.getAttribute("moduleID");
                    this.bookID = e.getAttribute("bookID");
                    this.chapterID = e.getAttribute("chapterID");
                    this.endVerseText = e.textContent;
                    break;
                } else {
                    it = e;
                }
            }
            this.selectedText = window.getSelection().toString();
            
        }
	}
}