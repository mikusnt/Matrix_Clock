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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;


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
    public void tryAdd(ASCII_Char e) throws IllegalAccessException {
        for (ASCII_Char item : list) {
            if (e.getId() == item.getId())
                throw new IllegalAccessException("Id " + e.getId() + " is in the list");
        }
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
    public void remove(int id) {
        if (list.get(id).getId() == getLastId()) {
            removeLast();
        } else if (list.get(id).getId() == getFirstId()) {
            removeFirst();
            
        } else {
            list.set(id, new ASCII_Char(id));
        }
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
            out += item.toCSVLine() + "\n";
        }
        return out;
    }
    
    @Override
    public Iterator<ASCII_Char> iterator() {
        return list.iterator();
    }
}
