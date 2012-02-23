package jslr;

import java.util.*;
import java.io.*;

/*
   Copyright 2006 Ondrej Lhotak. All rights reserved.

   Permission is granted for study use by
   students registered in CS 241, Spring 2008
   term.

   The contents of this file may not be
   published, in whole or in part, in print
   or electronic form.

   The contents of this file may be included
   in work submitted for CS 241 assignments in
   Spring 2008.  The contents of this file may
   not be submitted, in whole or in part, for
   credit in any other course.

*/

/*
 * JSLR constructs SLR(1) parse tables from a grammar, using the      
 * algorithms described in chapter 3 of Appel, _Modern Compiler       
 * Implementation in Java, second edition_, 2002. JSLR reads a grammar
 * on standard input, and writes the generated grammar on standard    
 * output.                                                            
 * 
*/

/** Represents an item (a dotted production). */
class Item {
    Production rule; // the production
    int pos; // position of the dot (0 <= pos <= rule.rhs.size())
    public Item( Production rule, int pos ) {
        this.rule = rule;
        this.pos = pos;
    }
    /** Returns true if the dot is not at the end of the RHS. */
    public boolean hasNextSym() {
        return pos < rule.rhs.length;
    }
    /** Returns the symbol immediately after the dot. */
    public String nextSym() {
        if(!hasNextSym()) throw new RuntimeException("Internal error: getting next symbol of an item with no next symbol");
        return rule.rhs[pos];
    }
    /** Returns the item obtained by advancing (shifting) the dot by one
     *  symbol. */
    public Item advance() {
        if(!hasNextSym()) throw new RuntimeException("Internal error: advancing an item with no next symbol");
        return new Item(rule, pos+1);
    }
    public boolean equals(Object other) {
        if(!(other instanceof Item)) return false;
        Item o = (Item) other;
        if(!rule.equals(o.rule)) return false;
        if(pos != o.pos) return false;
        return true;
    }
    public int hashCode() {
        return rule.hashCode() + pos;
    }
    public String toString() {
        StringBuffer ret = new StringBuffer();
        ret.append(rule.lhs);
        ret.append(" ->");
        int i;
        for(i = 0; i < pos; i++) ret.append(" "+rule.rhs[i]);
        ret.append(" ##");
        for(; i < rule.rhs.length; i++) ret.append(" "+rule.rhs[i]);
        return ret.toString();
    }
}
/** Represents a parser action (shift or reduce). */
abstract class Action {
}
/** Represents a shift parser action. */
class ShiftAction extends Action {
    Set<Item> nextState; // the automaton state to move to after the shift
    public ShiftAction(Set<Item> nextState) {
        this.nextState = nextState;
    }
    public int hashCode() { return nextState.hashCode(); }
    public boolean equals(Object other) {
        if(!(other instanceof ShiftAction)) return false;
        ShiftAction o = (ShiftAction) other;
        return nextState.equals(o.nextState);
    }
    public String toString() {
        return "shift " + nextState;
    }
}
/** Represents a reduce parser action. */
class ReduceAction extends Action {
    Production rule; // the production to reduce by
    public ReduceAction(Production rule) {
        this.rule = rule;
    }
    public int hashCode() { return rule.hashCode(); }
    public boolean equals(Object other) {
        if(!(other instanceof ReduceAction)) return false;
        ReduceAction o = (ReduceAction) other;
        return rule.equals(o.rule);
    }
    public String toString() {
        return "reduce " + rule;
    }
}
/** Utility class representing a pair of arbitrary objects. */
class Pair<A,B> {
    public Pair( A o1, B o2 ) { this.o1 = o1; this.o2 = o2; }
    public int hashCode() {
        return o1.hashCode() + o2.hashCode();
    }
    public boolean equals( Object other ) {
        if( other instanceof Pair ) {
            Pair p = (Pair) other;
            return o1.equals( p.o1 ) && o2.equals( p.o2 );
        } else return false;
    }
    public String toString() {
        return "Pair "+o1+","+o2;
    }
    public A getO1() { return o1; }
    public B getO2() { return o2; }

    protected A o1;
    protected B o2;
}


/** The main Jslr class. */
public class Jslr {
    /** The context-free grammar. */
    Grammar grammar;
    /** A map from each non-terminal to the productions that expand it. */
    Map<String,List<Production>> lhsToRules = new HashMap<String,List<Production>>();

    public Jslr(Grammar grammar) {
        this.grammar = grammar;
        for(Production p : grammar.productions) {
            List<Production> mapRules = lhsToRules.get(p.lhs);
            if(mapRules == null) {
                mapRules = new ArrayList<Production>();
                lhsToRules.put(p.lhs, mapRules);
            }
            mapRules.add(p);
        }
    }

    /** Compute the closure of a set of items using the algorithm of
     * Appel, p. 60 */
    public Set<Item> closure(Set<Item> i) {
        boolean change;
        while(true) {
            Set<Item> oldI = new HashSet<Item>(i);
            for( Item item : oldI ) {
                if(!item.hasNextSym()) continue;
                String x = item.nextSym();
                if(grammar.isTerminal(x)) continue;
                for( Production r : lhsToRules.get(x)) {
                    i.add(new Item(r, 0));
                }
            }
            if( i.equals(oldI) ) return i;
        }
    }
    /** Compute the goto set for state i and symbol x, using the algorithm
     * of Appel, p. 60 */
    public Set<Item> goto_(Set<Item> i, String x) {
        Set<Item> j = new HashSet<Item>();
        for( Item item : i ) {
            if( !item.hasNextSym() ) continue;
            if( !item.nextSym().equals(x) ) continue;
            j.add(item.advance());
        }
        return closure(j);
    }
    /** Add the action a to the parse table for the state state and
     * symbol sym. Report a conflict if the table already contiains
     * an action for the same state and symbol. */
    private boolean addAction( Set<Item> state, String sym, Action a ) {
        boolean ret = false;
        Pair<Set<Item>,String> p = new Pair<Set<Item>,String>(state, sym);
        Action old = table.get(p);
        if(old != null && !old.equals(a)) {
            throw new Error(
                "Conflict on symbol "+sym+" in state "+state+"\n"+
                "Possible actions:\n"+
                old+"\n"+a);
        }
        if(old == null || !old.equals(a)) ret = true;
        table.put(p, a);
        return ret;
    }
    /** Return true if all the symbols in l are in the set nullable. */
    private boolean allNullable(String[] l) {
        return allNullable(l, 0, l.length);
    }
    /** Return true if the symbols start..end in l are in the set nullable. */
    private boolean allNullable(String[] l, int start, int end) {
        boolean ret = true;
        for(int i = start; i < end; i++) {
            if(!nullable.contains(l[i])) ret = false;
        }
        return ret;
    }
    // The NULLABLE, FIRST, and FOLLOW sets. See Appel, pp. 47-49
    Set<String> nullable = new HashSet<String>();
    Map<String,Set<String>> first = new HashMap<String,Set<String>>();
    Map<String,Set<String>> follow = new HashMap<String,Set<String>>();
    /** Computes NULLABLE, FIRST, and FOLLOW sets using the algorithm
     * of Appel, p. 49 */
    public void computeFirstFollowNullable() {
        for( String z : grammar.syms() ) {
            first.put(z, new HashSet<String>());
            if(grammar.isTerminal(z)) first.get(z).add(z);
            follow.put(z, new HashSet<String>());
        }
        boolean change;
        do {
            change = false;
            for( Production rule : grammar.productions ) {
                if(allNullable(rule.rhs)) {
                    if( nullable.add(rule.lhs) ) change = true;
                }
                int k = rule.rhs.length;
                for(int i = 0; i < k; i++) {
                    if(allNullable(rule.rhs, 0, i)) {
                        if( first.get(rule.lhs).addAll(
                                first.get(rule.rhs[i])))
                            change = true;
                    }
                    if(allNullable(rule.rhs, i+1,k)) {
                        if( follow.get(rule.rhs[i]).addAll(
                                follow.get(rule.lhs)))
                            change = true;
                    }
                    for(int j = i+1; j < k; j++) {
                        if(allNullable(rule.rhs, i+1,j)) {
                            if( follow.get(rule.rhs[i]).addAll(
                                    first.get(rule.rhs[j])))
                                change = true;
                        }
                    }
                }
            }
        } while(change);
    }
    /** The computed parse table. */
    Map<Pair<Set<Item>,String>,Action> table = 
        new HashMap<Pair<Set<Item>,String>,Action>();
    Set<Item> initialState;
    /** Generates the SLR(1) parse table using the algorithms on 
     * pp. 60 and 62 of Appel. */
    public void generateTable() {
        Set<Item> startRuleSet = new HashSet<Item>();
        for(Production r : lhsToRules.get(grammar.start)) {
            startRuleSet.add(new Item(r, 0));
        }
        initialState = closure(startRuleSet);
        Set<Set<Item>> t = new HashSet<Set<Item>>();
        t.add(initialState);
        boolean change;
        // compute goto actions
        do {
            change = false;
            for( Set<Item> i : new HashSet<Set<Item>>(t) ) {
                for( Item item : i ) {
                    if(!item.hasNextSym()) continue;
                    String x = item.nextSym();
                    Set<Item> j = goto_(i, x);
                    if(t.add(j)) change = true;
                    if(addAction(i, x, new ShiftAction(j))) change = true;
                }
            }
        } while(change);
        // compute reduce actions
        for( Set<Item> i : t ) {
            for( Item item : i ) {
                if( item.hasNextSym() ) continue;
                for( String x : follow.get(item.rule.lhs) ) {
                    addAction(i, x, new ReduceAction(item.rule));
                }
            }
        }
    }
    /** Print the elements of a list separated by spaces. */
    public static String listToString(List l) {
        StringBuffer ret = new StringBuffer();
        boolean first = true;
        for( Object o : l ) {
            if( !first ) ret.append(" ");
            first = false;
            ret.append(o);
        }
        return ret.toString();
    }
    /** Produce output according to the output specification. */
    public void generateOutput() {
        Map<Production,Integer> ruleMap = new HashMap<Production,Integer>();
        int i = 0;
        for(Production r : grammar.productions) {
            ruleMap.put(r, i++);
        }
        Map<Set<Item>,Integer> stateMap = new HashMap<Set<Item>,Integer>();
        i = 0;
        stateMap.put(initialState, i++);
        for(Action a : table.values()) {
            if(!(a instanceof ShiftAction)) continue;
            Set<Item> state = ((ShiftAction)a).nextState;
            if(!stateMap.containsKey(state)) {
                stateMap.put(state, i++);
            }
        }
        for(Pair<Set<Item>,String> key : table.keySet()) {
            Set<Item> state = key.getO1();
            if(!stateMap.containsKey(state)) {
                stateMap.put(state, i++);
            }
        }
        System.out.println(i);
        System.out.println(table.size());
        for(Map.Entry<Pair<Set<Item>,String>,Action> e : table.entrySet()) {
            Pair<Set<Item>,String> p = e.getKey();
            System.out.print(stateMap.get(p.getO1())+" "+p.getO2()+" ");
            Action a = e.getValue();
            if(a instanceof ShiftAction) {
                System.out.println("shift "+
                        stateMap.get(((ShiftAction)a).nextState));
            } else if(a instanceof ReduceAction) {
                System.out.println("reduce "+
                        ruleMap.get(((ReduceAction)a).rule));
            } else throw new Error("Internal error: unknown action");
        }
    }
    public static final void main(String[] args) {
        Grammar grammar;
        try {
            grammar = Util.readGrammar(new Scanner(System.in));
            Util.writeGrammar(grammar);
        } catch(Error e) {
            System.err.println("Error reading grammar: "+e);
            System.exit(1);
            return;
        }
        Jslr jslr = new Jslr(grammar);
        try {
            jslr.computeFirstFollowNullable();
            jslr.generateTable();
            jslr.generateOutput();
        } catch(Error e) {
            System.err.println("Error performing SLR(1) construction: "+e);
            System.exit(1);
            return;
        } 
    }
}
/** A production in the grammar. */
class Production {
    public String lhs;
    public String[] rhs;
    public Production(String lhs, String[] rhs) {
        this.lhs = lhs; this.rhs = rhs;
    }
    public int hashCode() { return lhs.hashCode(); }
    public boolean equals(Object other) {
        if(!(other instanceof Production)) return false;
        Production o = (Production) other;
        if(!lhs.equals(o.lhs)) return false;
        if(rhs.length != o.rhs.length) return false;
        for(int i = 0; i < rhs.length; i++) {
            if(!rhs[i].equals(o.rhs[i])) return false;
        }
        return true;
    }
    public String toString() {
        StringBuffer ret = new StringBuffer();
        ret.append(lhs);
        //ret.append(" ->");
        for(String sym : rhs) {
            ret.append(" "+sym);
        }
        return ret.toString();
    }
}
/** Representation of a context-free grammar. */
class Grammar {
    Set<String> terminals = new LinkedHashSet<String>();
    Set<String> nonterminals = new LinkedHashSet<String>();
    Set<Production> productions = new LinkedHashSet<Production>();
    String start;

    public boolean isTerminal(String s) {
        return terminals.contains(s);
    }
    public boolean isNonTerminal(String s) {
        return nonterminals.contains(s);
    }
    public List<String> syms() {
        List<String> ret = new ArrayList<String>();
        ret.addAll(terminals);
        ret.addAll(nonterminals);
        return ret;
    }
}
class Error extends RuntimeException {
    public Error(String s) { super(s); }
}
class Util {
    public static String readLine(Scanner in, String msg) {
        if(!in.hasNextLine()) throw new Error(msg+" but input file ended");
        return in.nextLine();
    }
    public static int toInt(String line, String msg) {
        try {
            return new Integer(line);
        } catch(NumberFormatException e) {
            throw new Error("Expecting "+msg+" but the line is not a number:\n"+line);
        }
    }
    public static Grammar readGrammar(Scanner in) {
        Grammar grammar = new Grammar();
        String line = readLine(in, "Expecting number of non-terminals");
        int nterm = toInt(line, "number of non-terminals");
        for(int i = 0; i < nterm; i++) {
            grammar.terminals.add(readLine(in, "Expecting a non-terminal"));
        }
        if(grammar.terminals.size() != nterm) throw new Error("Duplicate terminals");

        line = readLine(in, "Expecting number of non-terminals");
        int nnonterm = toInt(line, "number of non-terminals");
        for(int i = 0; i < nnonterm; i++) {
            grammar.nonterminals.add(readLine(in, "Expecting a non-terminal"));
        }
        if(grammar.nonterminals.size() != nnonterm) throw new Error("Duplicate non-terminal");

        grammar.start = readLine(in, "Expecting start symbol");
        if(!grammar.nonterminals.contains(grammar.start)) throw new Error(
                "Start symbol "+grammar.start+" was not declared as a non-terminal.");

        line = readLine(in, "Expecting number of productions");
        int nprods = toInt(line, "number of productions");
        for(int i = 0; i < nprods; i++) {
            grammar.productions.add(readProduction(readLine(in, "Expecting production"), grammar));
        }
        if(grammar.productions.size() != nprods) throw new Error("Duplicate productions");
        return grammar;
    }
    public static Production readProduction(String line, Grammar grammar) {
        Scanner s = new Scanner(line);
        if(!s.hasNext()) throw new Error("Empty line instead of a production");
        String lhs = s.next();
        if(!grammar.isNonTerminal(lhs)) throw new Error("Symbol "+lhs+" was not declared as a non-terminal, but appears on the LHS of production "+line);
        List<String> rhs = new ArrayList<String>();
        while(s.hasNext()) {
            String sym = s.next();
            if(!grammar.isNonTerminal(sym) && !grammar.isTerminal(sym)) {
                throw new Error("Symbol "+sym+" is not a part of the grammar");
            }
            rhs.add(sym);
        }
        return new Production(lhs, 
                (String[]) rhs.toArray(new String[rhs.size()]));
    }
    static String checkIndent(String line, int indent) {
        for(int i = 0; i < indent; i++) {
            if(line.length() <= i) throw new Error("Expecting production but got empty line.");
            if(line.charAt(i) != ' ') throw new Error("Production "+line.substring(i)+" should be indented "+indent+" space(s), but it is indented "+i+" spaces");
        }
        if(line.length() <= indent) throw new Error("Expecting production but got empty line.");
        if(line.charAt(indent) == ' ') throw new Error("Production "+line+" should be indented "+indent+" spaces, but it is indented more than that.");
        return line.substring(indent);
    }
    static void printGrammar(Grammar grammar) {
        System.out.println("Terminals:");
        for(String s : grammar.terminals) {
            System.out.println("   "+s);
        }
        System.out.println();

        System.out.println("Nonterminals:");
        for(String s : grammar.nonterminals) {
            System.out.println("   "+s);
        }
        System.out.println();

        System.out.println("Start Symbol:");
        System.out.println("   "+grammar.start);
        System.out.println();

        System.out.println("Production Rules:");
        for(Production s : grammar.productions) {
            System.out.println("   "+s);
        }
    }
    static void writeGrammar(Grammar grammar) {
        System.out.println(grammar.terminals.size());
        for(String s : grammar.terminals) {
            System.out.println(s);
        }
        System.out.println(grammar.nonterminals.size());
        for(String s : grammar.nonterminals) {
            System.out.println(s);
        }
        System.out.println(grammar.start);
        System.out.println(grammar.productions.size());
        for(Production s : grammar.productions) {
            System.out.println(s);
        }
    }
}

