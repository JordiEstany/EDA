#include "Player.hh"
#include <algorithm>
#include <map>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME CoreDumped3

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME; 
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<int> VI;
  typedef vector<vector<bool> > mat_visitado;
  typedef vector<Pos> vec_pos;
  typedef vector<vector<int>> VVI; 

  VVI matriu_enemies;
  VVI matriu_caves;
  VVI matriu_trasures;
  VVI matriu_dwarves;
  VVI matriu_cel;
  VI busca_tresors;

  void matriz_enemies() {

    VI enemies_d_id;
    vec_pos enemies_pos;
    
    for (int i = 0; i < 4; i++) {
      if (i != me()) {
        VI aux = dwarves(i);
        enemies_d_id.insert(enemies_d_id.end(), aux.begin(), aux.end());
      }
    }
    int n = enemies_d_id.size();
    for (int i = 0; i < n; i++) {
      enemies_pos.push_back(unit(enemies_d_id[i]).pos);
    }

    VVI distancias (60, VI(60, -1));
    queue<Pos> Q;
    Pos p1;
    int en = enemies_pos.size();
    for (int i = 0; i < en; i++) {
      Q.push(enemies_pos[i]);
      distancias[enemies_pos[i].i][enemies_pos[i].j] = 0;
    }
    while (Q.size() > 0) {
      p1 = Q.front();
      Q.pop();
      
      int k = 0;
      while (k < 8) {
        Pos aux;
        aux = p1 + Dir(k);
        int dist = 0;
        if(pos_ok(aux)) {
          if(cell(aux).type == Cave or cell(aux).type == Outside) dist = distancias[p1.i][p1.j] + 1;
        else if(cell(aux).type == Rock) dist = distancias[p1.i][p1.j] + 6;
        else dist = distancias[p1.i][p1.j] + 1000;
        }        
        if(pos_ok(aux) and (distancias[aux.i][aux.j] == -1 or dist < distancias[aux.i][aux.j])) {
          Q.push(aux);
          distancias[aux.i][aux.j] = dist;
        }
        ++k;
      }
    }
    matriu_enemies = distancias;
    /*
    for (int i= 0; i < 60; i++) {
      cout << endl;
      for (int j = 0; j < 60; j++) {
        cout << distancias[i][j] << " ";
      }
    }*/
  }

  void matriz_cielo() {
    vec_pos cielo_pos;    
    
    for (int i = 0; i < 60; i++) {
      for (int j = 0; j < 60; j++) {
        Pos x;
        x.i = i;
        x.j = j;
        if (cell(x).type == Outside) cielo_pos.push_back(x);
      }
    }

    VVI distancias (60, VI(60, -1));
    queue<Pos> Q;
    Pos p1;
    int en = cielo_pos.size();
    for (int i = 0; i < en; i++) {
      Q.push(cielo_pos[i]);
      distancias[cielo_pos[i].i][cielo_pos[i].j] = 0;
    }
    int ant = 1;
    while (Q.size() > 0) {
      p1 = Q.front();
      Q.pop();
      
      int k = 0;
      while (k < 8) {
        Pos aux;
        aux = p1 + Dir(k);
        int dist = 0;
        if(pos_ok(aux)) {
          if(cell(aux).type == Cave or cell(aux).type == Outside) dist = distancias[p1.i][p1.j] + 1;
        else if(cell(aux).type == Rock) dist = distancias[p1.i][p1.j] + 6;
        else dist = distancias[p1.i][p1.j] + 1000;
        }        
        if(pos_ok(aux) and (distancias[aux.i][aux.j] == -1 or dist < distancias[aux.i][aux.j])) {
          Q.push(aux);
          distancias[aux.i][aux.j] = dist;
        }
        ++k;
      }
    }
    matriu_cel = distancias;    
  }
  void matriz_caves() {
    vec_pos caves_pos;
    
    
    for (int i = 0; i < 60; i++) {
      for (int j = 0; j < 60; j++) {
        Pos x;
        x.i = i;
        x.j = j;
        if (cell(x).type == Cave and cell(x).owner != me()) caves_pos.push_back(x);
      }
    }

    VVI distancias (60, VI(60, -1));
    queue<Pos> Q;
    Pos p1;
    int en = caves_pos.size();
    for (int i = 0; i < en; i++) {
      Q.push(caves_pos[i]);
      distancias[caves_pos[i].i][caves_pos[i].j] = 0;
    }
    int ant = 1;
    while (Q.size() > 0) {
      p1 = Q.front();
      Q.pop();
      
      int k = 0;
      while (k < 8) {
        Pos aux;
        aux = p1 + Dir(k);
        int dist = 0;
        if(pos_ok(aux)) {
          if(cell(aux).type == Cave or cell(aux).type == Outside) dist = distancias[p1.i][p1.j] + 1;
        else if(cell(aux).type == Rock) dist = distancias[p1.i][p1.j] + 6;
        else dist = distancias[p1.i][p1.j] + 1000;
        }        
        if(pos_ok(aux) and (distancias[aux.i][aux.j] == -1 or dist < distancias[aux.i][aux.j])) {
          Q.push(aux);
          distancias[aux.i][aux.j] = dist;
        }
        ++k;
      }
    }
    matriu_caves = distancias;
    /*
    for (int i= 0; i < 60; i++) {
      cout << endl;
      for (int j = 0; j < 60; j++) {
        cout << matriu_caves[i][j] << " ";
      }
    }*/
  }

void matriz_treasures() {
    vec_pos tres_pos;
    
    
    for (int i = 0; i < 60; i++) {
      for (int j = 0; j < 60; j++) {
        Pos x;
        x.i = i;
        x.j = j;
        if (cell(x).treasure) tres_pos.push_back(x);
      }
    }

    VVI distancias (60, VI(60, -1));
    queue<Pos> Q;
    Pos p1;
    int en = tres_pos.size();
    for (int i = 0; i < en; i++) {
      Q.push(tres_pos[i]);
      distancias[tres_pos[i].i][tres_pos[i].j] = 0;
    }
    int ant = 1;
    while (Q.size() > 0) {
      p1 = Q.front();
      Q.pop();
      
      int k = 0;
      while (k < 8) {
        Pos aux;
        aux = p1 + Dir(k);
        int dist = 0;
        if(pos_ok(aux)) {
          if(cell(aux).type == Cave or cell(aux).type == Outside) dist = distancias[p1.i][p1.j] + 1;
        else if(cell(aux).type == Rock) dist = distancias[p1.i][p1.j] + 6;
        else dist = distancias[p1.i][p1.j] + 1000;
        }        
        if(pos_ok(aux) and (distancias[aux.i][aux.j] == -1 or dist < distancias[aux.i][aux.j])) {
          Q.push(aux);
          distancias[aux.i][aux.j] = dist;
        }
        ++k;
      }
    }
    matriu_trasures = distancias;
    /*
    for (int i= 0; i < 60; i++) {
      cout << endl;
      for (int j = 0; j < 60; j++) {
        cout << matriu_trasures[i][j] << " ";
      }
    }*/
  }

  void matriz_dwarves() {
    vec_pos dw_pos;    
    
    VI D = dwarves(me());
    int n = D.size();
    for (int i = 0; i < n; ++i) {
        int id = D[i];
        dw_pos.push_back(unit(id).pos);
    }

    VVI distancias (60, VI(60, -1));
    queue<Pos> Q;
    Pos p1;
    int en = dw_pos.size();
    for (int i = 0; i < en; i++) {
      Q.push(dw_pos[i]);
      distancias[dw_pos[i].i][dw_pos[i].j] = 0;
    }
    int ant = 1;
    while (Q.size() > 0) {
      p1 = Q.front();
      Q.pop();
      
      int k = 0;
      while (k < 8) {
        Pos aux;
        aux = p1 + Dir(k);
        int dist = 0;
        if(pos_ok(aux)) {
          if(cell(aux).type == Cave or cell(aux).type == Outside) dist = distancias[p1.i][p1.j] + 1;
        else dist = distancias[p1.i][p1.j] + 1000;
        }        
        if(pos_ok(aux) and (distancias[aux.i][aux.j] == -1 or dist < distancias[aux.i][aux.j])) {
          Q.push(aux);
          distancias[aux.i][aux.j] = dist;
        }
        ++k;
      }
    }
    matriu_dwarves = distancias;/*
    if (round () == 1) {
    for (int i= 0; i < 60; i++) {
      cout << endl;
      for (int j = 0; j < 60; j++) {
        cout << matriu_dwarves[i][j] << " ";
      }
    }
    }*/
  }


  void caza_tesortos() {
    vec_pos tres_pos;    
    for (int i = 0; i < 60; i++) {
      for (int j = 0; j < 60; j++) {
        Pos x;
        x.i = i;
        x.j = j;
        if (cell(x).treasure) tres_pos.push_back(x);
      }
    }
    int n = tres_pos.size();
    VI t_h(n);

    for (int i = 0; i < n; i++) {
      Pos p = tres_pos[i];
      bool trobat = false;
      if (dwarves(me()).size() > 0) {
        while (!trobat) {
        if ( matriu_dwarves[p.i][p.j] == 0) {
          trobat = true;
          t_h[i] = cell(p).id;
        }
        else {
          int k = 0;
          for (int i = 1; i < 8; i++) {
            Pos p2 = p + Dir(k);
            Pos p3 = p + Dir(i);
            if (!pos_ok(p2)) k = i;
            else if (pos_ok(p3) and matriu_dwarves[p2.i][p2.j] > matriu_dwarves[p3.i][p3.j]) k = i;
          }
          p = p + Dir(k);
        }        
      } 
      }
       
    }
    
    busca_tresors =t_h;
  }

  void attack_dwarves() {
    VI D = dwarves(me());
    int n = D.size();
    for (int i = 0; i < n; ++i) {
        bool enemy = false;
        int id = D[i];
        for (int k = 0; not enemy and k < 8; ++k) {
          Pos p = unit(id).pos + Dir(k);
          if (pos_ok(p)) {
            int id2 = cell(p).id;
            if (id2 != -1 and unit(id2).player != me()) {
                enemy = true;
              command(id, Dir(k));
            }
          }
        }        
      }
    }

void move_dwarves() {
    VI D = dwarves(me());
    int n = D.size();
    for (int i = 0; i < n; ++i) {
        int id = D[i];
        Pos p = unit(id).pos;
        int aux = dwarve_busca_tresors(id);
        int aux2 = moves_dwarve2(id);
        if (aux == -1) command(id, Dir(aux2));   
        else if (matriu_trasures[p.i][p.j]/4 < matriu_caves[p.i][p.j]) {
          command(id, Dir(aux));
          }
        else command(id, Dir(aux2));     
    }

}

/*
int moves_dwarve(int id) {
                Pos dwarve_pos = unit(id).pos;
                queue<Pos> Q;
                bool direccion_encontrada = false;
                Q.push(dwarve_pos);
                mat_visitado visitats(60, vector<bool>(60, false));

                visitats[dwarve_pos.i][dwarve_pos.j] = true;
                Pos p1;

                while(Q.size() > 0 and !direccion_encontrada) {
                    p1 = Q.front();
                    Q.pop();
                    Pos x;
                    if(cell(p1).owner != me() and cell(p1).type == Cave and cell(p1).id == -1) {
                        direccion_encontrada = true;
                    }
                    else {
                        int k = 0;
                        while (k < 8 and !direccion_encontrada) {
                            Pos aux;
                            aux = p1 + Dir(k);
                            if(pos_ok(aux) and !visitats[aux.i][aux.j]) {
                                visitats[aux.i][aux.j] = true;
                                Q.push(aux);
                            }
                            ++k;
                        }
                    }

                }

                if(p1.i < dwarve_pos.i) {
                    if (p1.j < dwarve_pos.j) return 5;
                    else if(p1.j > dwarve_pos.j) return 3;
                    else command(id, Dir(4));
                }
                else if(p1.i > dwarve_pos.i) {
                    if (p1.j < dwarve_pos.j) return 7;
                    else if(p1.j > dwarve_pos.j) return 1;
                    else command(id, Dir(0));
                }
                else {
                    if (p1.j < dwarve_pos.j) return 6;
                    else if(p1.j > dwarve_pos.j) return 2;
                }

}*/

int dwarve_busca_tresors(int& id) {
        bool hunter = false;
        int bs = busca_tresors.size();
        for (int i = 0; i < bs; i++) {
          if (busca_tresors[i] == id) hunter = true;
        }
        if (hunter) {
          int k = 0;
          Pos p = unit(id).pos;
          for (int i = 1; i < 8; i++) {
            Pos p2 = p + Dir(k);
            Pos p3 = p + Dir(i);
            if (!pos_ok(p2)) k = i;
            else if (pos_ok(p3) and matriu_trasures[p2.i][p2.j] > matriu_trasures[p3.i][p3.j]) k = i;
          }
          return k;
        }
        else return -1;         
    
}

int moves_dwarve2 (int& id) {        
  int k = 0;
  Pos p = unit(id).pos;
  for (int i = 1; i < 8; i++) {
    Pos p2 = p + Dir(k);
    Pos p3 = p + Dir(i);
    if (!pos_ok(p2)) k = i;
    else if ( pos_ok(p3) and matriu_caves[p2.i][p2.j] > matriu_caves[p3.i][p3.j] and unit(cell(p3).id).player != me()) k = i;
  }
  return k;    
}
void dodger_run(int& id) {
  int k = 0;
  Pos p = unit(id).pos;
  for (int i = 1; i < 8; i++) {
    Pos p2 = p + Dir(k);
    Pos p3 = p + Dir(i);
    if (!pos_ok(p2)) k = i;
    else if ( pos_ok(p3) and matriu_cel[p2.i][p2.j] > matriu_cel[p3.i][p3.j]) k = i;
  }
  command(id, Dir(k));    

}
void dodge_balrog() {
  int dodger = 0;
  int bal = balrog_id();
  Pos bal_pos = unit(bal).pos;
  queue<Pos> Q;
  bool direccion_encontrada = false;
  Q.push(bal_pos);
  mat_visitado visitats_b(60, vector<bool>(60, false));

  visitats_b[bal_pos.i][bal_pos.j] = true;
  Pos p1;

  while(Q.size() > 0 and !direccion_encontrada) {
    p1 = Q.front();
    Q.pop();
    Pos x;
    int id1 = cell(p1).id; 
    if(id1 != -1 and unit(id1).player > 0 and unit(id1).player < 4 ) {
      direccion_encontrada = true;
      dodger = id1;
    }
    else {
      int k = 0;
      while (k < 8 and !direccion_encontrada) {
        Pos aux;
        aux = p1 + Dir(k);
        if(pos_ok(aux) and !visitats_b[aux.i][aux.j]) {
          visitats_b[aux.i][aux.j] = true;
          Q.push(aux);
        }
        ++k;
      }
    }

  }
  if (unit(dodger).player == me()) dodger_run (dodger);
}
int moves_wizard2 (int& id) {        
  int k = 0;
  Pos p = unit(id).pos;
  for (int i = 1; i <= 8; i++) {
    Pos p2 = p + Dir(k);
    Pos p3 = p + Dir(i);
    if (!pos_ok(p2) or cell(p2).type == (Cave or Outside) and cell(p2).id != -1) k = i;
    else if ( pos_ok(p3) and matriu_dwarves[p2.i][p2.j] > matriu_dwarves[p3.i][p3.j] and matriu_dwarves[p3.i][p3.j] != -1) k = i;
  }
  return k;    
}
void move_wizards() {
    VI W = wizards(me());
    int n = W.size();
    for (int i = 0; i < n; ++i) {
        int id = W[i];
        command(id, Dir(moves_wizard2(id)));          
    }
}
  /**
   * Play method, invoked once per each round.
   */
  void play () {
    
    matriz_dwarves();
    caza_tesortos();
     matriz_enemies();
     matriz_caves();
     matriz_cielo();
     matriz_treasures();
     attack_dwarves();
     dodge_balrog();     
     move_dwarves();   
     move_wizards();   
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
