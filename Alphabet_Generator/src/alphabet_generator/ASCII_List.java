/*
 * Copyright (C) 2018 MS-1
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package alphabet_generator;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author MS-1
 */
public class ASCII_List implements Iterable<ASCII_Char>{
    private final List<ASCII_Char> list;

    public ASCII_List() {
        this.list = new ArrayList<>();
    }
    private int getFirstId() {
        return list.get(0).getId();
    }
    private int getLastId() {
        return list.get(list.size()-1).getId();
    }
    public boolean isIdInList(int newId) {
        for (ASCII_Char item : list) {
            if (newId == item.getId())
                return true;
        }
        return false;
    }
    public void tryAdd(ASCII_Char e) throws IllegalAccessException {
        if (isIdInList(e.getId()))
                throw new IllegalAccessException("Id " + e.getId() + " is in the list");
        if (list.isEmpty())
            list.add(e);
        int firstId = getFirstId();
        int lastId = getLastId();
        if (e.getId() < firstId) {
            list.add(e);
            for (int i = e.getId() + 1; i < firstId; i++) 
                list.add(new ASCII_Char(i));
        } else if (e.getId() > lastId) {
            for(int i = lastId + 1; i < e.getId(); i++) {
                list.add(new ASCII_Char(i));
            }
            list.add(e);
        }
        list.sort((ASCII_Char o1, ASCII_Char o2) -> o1.getId() - o2.getId());
        Collections.sort(list);
    }
    private void removeLast() {
        if (list.size() > 0) {
            list.remove(list.size()-1);
        }
    }
    private void removeFirst() {
        if (list.size() > 0)
            list.remove(0);
    }
    public void remove(int index) {
        if (list.size() > 0) {
            if (list.get(index).getId() == getLastId()) {
                removeLast();
            } else if (list.get(index).getId() == getFirstId()) {
                removeFirst();

            } else {
                list.set(index, new ASCII_Char(list.get(index).getId()));
            }
        }
    }
    public ASCII_Char get(int index) {
        return list.get(index);
    }
    public String toCSVLine(int id) {
        if (id < list.size())
            return list.get(id).toCSVLine();
        else
            return "";
    }
    
    public int getListSize() {
        return list.size();
    }
    
    @Override
    public String toString() {
        String out = "";
        for (ASCII_Char item : list) {
            out += item.toCSVLine();
        }
        return out;
    }
    
    @Override
    public Iterator<ASCII_Char> iterator() {
        return list.iterator();
    }
    
    public void saveToFile(String filename){
        File f = new File(filename);
        try (PrintWriter file = new PrintWriter(f)) {
            file.print(this.toString());
        } catch (Exception e) {
            System.out.println(filename + " not found");       
        }
    }
    
    public static ASCII_List readFromCSV(String filename) {
        ASCII_List newList = new ASCII_List();
        try {
            FileReader reader = new FileReader(filename);
            try (BufferedReader file = new BufferedReader(reader)) {
                String str;
                while ((str = file.readLine()) != null) {
                    newList.tryAdd(ASCII_Char.fromCSVLine(str));
                }
            }
        } catch (IOException e) {
            System.out.println(filename + " IO exception");
        } catch (IllegalAccessException ex) {
            Logger.getLogger(ASCII_List.class.getName()).log(Level.SEVERE, null, ex);
        }
        return newList;
    }
    
    public int getNextEmptyId(int index) {
        if (list.isEmpty())
            return 0;
        int id = list.get(index).getId();
        for(int i = index + 1; i < list.size(); i++) {
            if (list.get(i).getId() > (id + 1))
                return id + 1;
            id = list.get(i).getId();
        }
        return id + 1;
    }
    
    public void renameItemId(int index, int newId) {
        if (list.get(index).getId() != newId) {
            try {
                if (isIdInList(newId)) 
                    throw new IllegalAccessException("Id " + newId + " is in the list");
                ASCII_Char copy = new ASCII_Char(list.get(index), newId);
                list.remove(index);
                tryAdd(copy);
            } catch (IllegalAccessException e) {
                System.out.println(e.toString());
            }
        }
        
    }
}
