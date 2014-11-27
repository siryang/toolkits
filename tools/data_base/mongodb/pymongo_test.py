#-------------------------------------------------------------------------------
# Name:        pymongo_test
# Purpose:
#
# Author:      yangdi
#
# Created:     10/02/2014
# Copyright:   (c) yangdi 2014
# Licence:     <your licence>
#-------------------------------------------------------------------------------

import pymongo as mg
import sqlite3 as sq
import json

def openMongoTable(url, dbName, tableName):
    return mg.MongoClient(url)[dbName][tableName]

def openSqlConnect(url):
    return sq.connect(url)

i = 0
def store2Sql(sqConnect):
    cursor = sqConnect.cursor()
    cursor.execute("PRAGMA table_info(MongoCarModel);")
    fieldList = list()
    map(lambda x: fieldList.append(x[1]), cursor.fetchall())
    assert(len(fieldList) != 0)

    def store(item):
        global i
        if i % 100 == 0:
            print i
        i+=1
        
        value = reduce(lambda x, y: x + "," + y\
		    , map(lambda x: "'%s'" % item[x] if type(item[x]) == unicode\
			    else "'%s'" % str(item[x]) , fieldList))
        ##print value
        cmd = "insert into MongoCarModel values(" + value + ")"
        ##print cmd
        try:
            return sqConnect.execute(cmd)
        except:
            print "error"
            return None 

    return store

def main():
    collection = openMongoTable("mongodb://10.10.24.200:27017", "obd_base", "base_carmodel")
    rlt = collection.find({}\
        ,{"_id":"1"\
        , "_m_f_brand":"1"\
        , "_m_s_brand":"1"\
        , "_m_t_brand":"1"
        , "_logo":"1"\
        , "_m_year":"1"\
        , "_m_model":"1"\
        , "_fuel_capacity":"1"\
        , "_displacement_liter":"1"\
        , "_displacement_milliliter":"1"\
        , "_innercylinder_count":"1"\
        , "_fuel_type":"1"\
        , "_compres_ratio":"1"})
    sqlConnect = openSqlConnect("F:\proj\obd2\data\obd_sqlite.db")
    storeFunction = store2Sql(sqlConnect);
    map(storeFunction, rlt)
    sqlConnect.commit();
#    sqlConnect.close();
#    collection.close();
    print "done"

if __name__ == '__main__':
    main()

