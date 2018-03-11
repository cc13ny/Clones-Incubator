import datetime
from pymongo import MongoClient

def main():
    """ Connect to MongoDB """
    client = MongoClient('localhost', 27017)
    print "Connected Successfully"
    db = client.test_database
    print 
    print db
    collection = db.test_collection
    print 
    print collection

    posts = db.posts
    print
    print posts

    # You should run mongod --dbpath "database/path/youchoose/" on cmd
    post = {"author": "Mike",
            "text": "My first blog post!",
            "tags": ["mongodb", "python", "pymongo"],
            "date": datetime.datetime.utcnow()}
    result = posts.insert_one(post)
    post_id = result.inserted_id
    print
    print post_id

if __name__ == "__main__":
    main()
