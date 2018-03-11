from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.pdfparser import PDFParser
from pdfminer.pdfdocument import PDFDocument
from pdfminer.converter import TextConverter
from pdfminer.layout import LAParams
from pdfminer.pdfpage import PDFPage
from cStringIO import StringIO
import nltk

#-------------------- IO --------------------#
## extract Bookmarks or Outlines
def get_toc(path):
    infile = open(path, 'rb')
    parser = PDFParser(infile)
    doc = PDFDocument(parser)

    toc = list()
    for (level, title, dest, a, structelem) in doc.get_outline():
        toc.append((level, title))
    return toc
    
## convert pdf into txt
def conv_pdf2txt(path, pages=None):
    if not pages:
        pagenums = set()
    else:
        pagenums = set(pages)

    output = StringIO()
    manger = PDFResourceManager()
    codec = 'ascii' #or 'utf-8'
    device = TextConverter(manger, output, codec=codec, laparams=LAParams())
    interpreter = PDFPageInterpreter(manger, device)

    infile = file(path, 'rb')

    for page in PDFPage.get_pages(infile, pagenums, caching=True, check_extractable=True):
        interpreter.process_page(page)

    txt = output.getvalue()

    infile.close()
    device.close()
    output.close()
    return txt

#-------------------- Preprocess --------------------#
class Preprocess(object):
    def __init__(self, doc):
        self.doc = doc

    def correct_spell(self):
        pass
    
    def pdfminer_prep(self):
        
        #step 1: remove '\n''s which set alphas apart
        doc = ''.join(self.doc.split('\n'))
        words = doc.split()

        #step 2: merge two words in cases like 'return-' & 'ed'
        i = 0
        size = len(words)
        while i < size:
            word = words[i]
            if word[len(word) - 1] == '-':
                if i+1 < size:
                    words[i] = word[:-1] + words[i+1]
                    words.pop(i+1)
                    size-=1
            i+=1
        doc = ' '.join(words)

        #ToDo: adjust wrong orders of sents due to locs
        #      (e.g. by txt extract loc provided by PDFMiner)
        #ToDo: avoid bug due to character coding (e.g. by re)
        #ToDo: better sentence splitter
        #ToDo: extract data like tables or imgs
        #ToDo: use hadoop or cuda to parallelize pipes

        return doc
    def twit_clean(self):
        #Guide: http://www.analyticsvidhya.com/blog/2015/06/quick-guide-text-data-cleaning-python/
        #ToDo: clean unwanted html character
        #ToDo: adjust bad grammar and poor spelling
        doc = self.doc


#-------------------- Feature --------------------#

# Included:

#      1. Feature Extraction
#         1.1 bow: bag-of-words [Presence of Words]
#         1.2 tfidf: TfidfTransformer [Importance of Words]
class Feature(object):
    def __init__(self, doc):
        self.doc = doc
    def bow(self):
        # bag-of-words
        pass

    def tfidf(self):
        # Term Frequency Inverse Document Frequency
        pass
    

    def ngrams(self, n):
        pass
    

    def hinfo(self):
        # high information words
        pass
    

    def infoi(self):
        # information items
        pass

class Model(object):
    def __init__(self, doc):
        self.doc = doc
        
class Task(object):
    def __init__(self, doc):
        self.doc = doc

    def abs_summ(self):
        doc = self.doc
        ft = Feature(doc)
        init = ft.infoi()
        pass
        # abstractive summarization
        # ToDo: Text Planning

    def anal_twitter(self):
        pass

    def anal_senti(self):
        pass

    def brown_cluster(self):
        pass

    def classify_sent(self):
        pass
        # Model:
        #   1. CNN classification
        #   2. KNN Classification

    def det_fault(self):
        pass

    def get_topic(self):
        pass
        # Maybe it should be Feature (Extraction)
        # Model:
        #   1. LDA
    
    def nlp_gen(self):
        pass
        # nlp generation
        # ToDo: Make detailed specification
        # Model:
        #   1. char-level lstm
        
    def pdf_demo(self):
        txt = self.doc
        pr = Preprocess(txt)
        doc = pr.pdfminer_prep()

        sents = nltk.sent_tokenize(doc)

        #import pprint
        #pprint.pprint(sents)
        for i, sent in enumerate(sents):
            print str(i+1) + ') ' + sent
            print
    def recomm(self):
        
pass
            
    def rm_amb(self):
        pass
    
    def txt_rank(self):
        # Maybe it should be Feature (Extraction)
        pass

    def twit_anal(self):
        pass
    
def main():
    txt = conv_pdf2txt(path='lsa.pdf', pages=[0])#raw txt on the first page
    tk = Task(txt)
    tk.pdf_demo()
    
if __name__ == "__main__":
    main()

#Direction:
#   1. More Processing
#      1.1 regular expression
#      1.2 Name Entity
#      1.3 POS
#      1.4 Language Model (n-gram, char-level, word-level)
#      1.5 Topic Model
#      1.6 Word Clustering
#      1.7 Text cleaning: like lowering, remove
#      1.8 Semantic Analysis based on LSTM
