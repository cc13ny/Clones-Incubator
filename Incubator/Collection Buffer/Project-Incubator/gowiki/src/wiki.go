//References:
// 1. https://golang.org/doc/articles/wiki/
// 2. https://blog.golang.org/go-slices-usage-and-internals
// 3. https://golang.org/doc/articles/wiki/part2.go

package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

type Page struct {
    Title string
    Body []byte
}

func (p *Page) save() error {
    filename := p.Title + ".txt"
    return ioutil.WriteFile(filename, p.Body, 0600)
}

func loadPage(title string) (*Page, error) {
    filename := title + ".txt"
    body, err := ioutil.ReadFile(filename)
    if err != nil {
        return nil, err
    }
    return &Page{Title: title, Body: body}, nil
}

//Website Handler

func viewHandler(w http.ResponseWriter, r *http.Request) {
    title := r.URL.Path[len("/view/"):]
    p, _ := loadPage(title)
    fmt.Fprintf(w, "<h1>%s</h1><div>%s</div>", p.Title, p.Body)
}

func editHandler(w http.ResponseWriter, r *http.Request) {
    title := r.URL.Path[len("/edit/"):]
    p, err := LoadPage(title)
    if err != nil {
        p = &Page{Title: title}
        fmt.Fprintf(w, "<h1>Editing %s</h1>" +
            "<form action=\"/save/%s\" method=\"POST\">"+
            "<textarea name=\"body\">%s</textarea><br
    }
}

func main() {
    p1 := &Page{Title: "test", Body:[]byte("Hello World!")}
    p1.save()

    http.HandleFunc("/view/", viewHandler)
    http.HandleFunc("/edit/", editHandler)
    http.HandleFunc("/save/", saveHandler)
    http.ListenAndServe(":8080", nil)
}