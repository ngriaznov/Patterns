 
  bool euclidian(int c, int k, int n, int r)
  {
    /*  generates euclideian patterns 
        c: current step number 
        k: hits per bar 
        n: bar length 
        r: rotation 
        returns true or false according to the euclidean pattern*/ 
    
    return (((c + r) * k) % n) < k; 
  }
