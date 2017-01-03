import threading
from queue import Queue
from spider import Spider
from domain import *
from general import *
from urllib.request import urlopen
import  chardet
import os

##############################################################
#some global setting
PROJECT_NAME = 'nju'
HOMEPAGE = 'http://www.nju.edu.cn/'
DOMAIN_NAME = get_domain_name(HOMEPAGE)
QUEUE_FILE = PROJECT_NAME + '/queue.txt'
CRAWLED_FILE = PROJECT_NAME + '/crawled.txt'
NUMBER_OF_THREADS = 4
queue = Queue()
##############################################################



global ALLNUM
ALLNUM=0
init_file=open(PROJECT_NAME+'/pagelist.txt','r')
lines=init_file.readlines()
ALLNUM=len(lines)
init_file.close()




Spider(PROJECT_NAME, HOMEPAGE, DOMAIN_NAME)

lock=threading.Lock()

def append_link(url):
    if(not os.path.exists(PROJECT_NAME+'/pagelist.txt')):
        file=open(PROJECT_NAME+'/pagelist.txt','w')
    else:
        file=open(PROJECT_NAME+'/pagelist.txt','a')
    file.write(url+'\n')
    file.close()


# Create worker threads (will die when main exits)
def create_workers():
    for _ in range(NUMBER_OF_THREADS):
        t = threading.Thread(target=work)
        t.daemon = True
        t.start()


# Do the next job in the queue
def work():
    while True:
        url = queue.get()
        global ALLNUM
        try:
            if (lock.acquire()):
                response=urlopen(url,timeout=3)
                childrenlink = Spider.gather_links(url)
                childrenfile = open(PROJECT_NAME + '/childrenlink/' + str(ALLNUM) + '.txt', 'w')
                childrenfile.write(url + '\n')
                for each_child in childrenlink:
                    if ('javascript' not in each_child):
                        childrenfile.write(each_child + '\n',)
                childrenfile.close()

                #write html file by utf8 encoding
                html_byte=response.read()
                chardit1 = chardet.detect(html_byte)
                file1 = open(PROJECT_NAME + '/html/utf8/' + str(ALLNUM) + '.html', 'wb')
                html_string=html_byte.decode(chardit1['encoding']).encode('utf-8')
                file1.write(html_string)
                file1.close()
				
				#for smj encode as GBK
                file2 = open(PROJECT_NAME + '/html/gbk/' + str(ALLNUM) + '.html', 'wb')
                html_string = html_byte.decode(chardit1['encoding'], 'ignore').encode('gbk','ignore')
                file2.write(html_string)
                file2.close()

        except Exception as e:
            print(str(e))
            queue.task_done()
            lock.release()
        else:
            append_link(url)
            ALLNUM = ALLNUM + 1
            Spider.crawl_page(threading.current_thread().name, url)
            queue.task_done()
            lock.release()

# Each queued link is a new job
def create_jobs():
    for link in file_to_set(QUEUE_FILE):
        queue.put(link)
    queue.join()
    crawl()


# Check if there are items in the queue, if so crawl them
def crawl():
    queued_links = file_to_set(QUEUE_FILE)
    if len(queued_links) > 0:
        print(str(len(queued_links)) + ' links in the queue')
        create_jobs()


create_workers()
crawl()
