# Google Dapper
## Question
1. In _Abstract_, what is called __Application-Level Transparency__?
   + Programmers shouldn't need to be aware of the system.
2. How does tracing system introduce __Latency__?
3. Why __Application-Level Transparency__?
   + Shouldn't rely on application-level developers to function, which violates the ubiquity requirement.
4. Why __Scalability__?
   + Future-Oriented Programming
## Requirements
1. __First & Formost__
   + Ubiquitous development
   + Continuous monitoring
2. __Performance Related__
   + Low Overhead
   + Application-Level Transparency
   + Scalability
3. __Other Features__
   + Real-time Data Analysis
   
### Summary
+ Current
  + __MVP__ (itself): Ubiquitous & Continuous
  + __Decoupling__ (others): Low Overhead & ALT
+ Future
  + __Scalability__
## Points
1. Key successful points
   + Sampling
   + Restricting the instrumentation to a rather small number of common libraries
## Use Cases
## Reference
1. [Magpie](https://www.microsoft.com/en-us/research/publication/magpie-online-modelling-and-performance-aware-systems/) ([link1](https://www.microsoft.com/en-us/research/wp-content/uploads/2003/05/magpiehotos03.pdf), [link2](https://ai2-s2-pdfs.s3.amazonaws.com/cb76/9b0c983bce6a73cafd1f8a346995bd902fa9.pdf))
2. [X-trace](https://people.eecs.berkeley.edu/~istoica/papers/2007/xtr-nsdi07.pdf) ([Github](https://github.com/rfonseca/X-Trace), [Manual](http://www.x-trace.net/wiki/doku.php))
3. [Google Cluster Arch](https://static.googleusercontent.com/media/research.google.com/en//archive/googlecluster-ieee.pdf)
4. [Universal Search](https://googleblog.blogspot.com/2007/05/universal-search-best-answer-is-still.html)
