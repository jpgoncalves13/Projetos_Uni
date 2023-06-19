{- | 
 - O objetivo desta Tarefa é fazer com que os fantasmas tenham um comportamento próprio no jogo, ou seja, que vão atrás do Pacman ou que fujam dele.

 - Em primeiro lugar, decidimos fazer uma função que retira as coordenadas do Pacman de uma lista de Players, para depois comparar com as coordenadas do Ghost.
   Retirando as coordenadas do Pacman, fizemos as funções scatterPac e chasePac, que dado um State, um Inteiro e coordenadas produz um Play. Com a função scatterPac, o objetivo é dar instruções para o Ghost fugir do Pacman, comparando as suas coordenadas e vendo se a Piece a seguir é Wall . 
   Se for Wall, então dependendo da sua orientação o Ghost vai se movimentar sempre para a sua direita para não ir contra a Wall. A função chasePac vai ter quase o mesmo mecanismo que a função scatterPac só que o objetivo nesta é dar instruções ao Ghost para ir atrás do Pacman.
   De seguida, criamos uma função chamada findGhosts1 que dado uma lista de Players, retorna outra lista sem o Pacman e só com Ghosts.
   A função chaseMode vai fazer uma jogada para o Ghost ir atrás do Pacman, e vai utilizar a função chasePac para esse efeito. Já a função scatterMode vai fazer uma jogada para o Ghost fugir do Pacman, utilizando a função scatterPac. 
   Para finalizar fizemos a função ghostPlay que dado um State, vai devolver as jogadas de todos os fantasmas presentes na lista de Players do State.

 - Concluindo, o resultado mais importante desta Tarefa foi a alteração do comportamento dos fantasmas -}


module Tarefa5 where 


import Types
import Tarefa2

-- | Coloca o pacman no topo da lista 
pacEnd :: [Player] -> [Player]
pacEnd [] = [] 
pacEnd ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):xs) = (Pacman (PacState (a,(x,y),b,c,d,e) f g h)) : xs
pacEnd ((Ghost (GhoState (a,(x,y),b,c,d,e) f)):xs) = pacEnd xs ++ [(Ghost (GhoState (a,(x,y),b,c,d,e) f))]


-- | Transforma uma lista de Players com Pacmans e Ghosts em uma lista de Players só com Ghosts
--
findGhosts1 :: [Player] -> [Player]
findGhosts1 [] = []
findGhosts1 ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = t
findGhosts1 (p1@(Ghost (GhoState (a,(x,y),b,c,d,e) f)):ys) = p1 : findGhosts1 ys

-- | Jogada para o Ghost ir atrás do Pacman
--
chaseMode :: State -> Int -> Play
chaseMode (State m w@((Ghost (GhoState (a,(x1,y1),b,c,d,e) Alive )):t) l) z = chasePac (State m (findGhosts1 w) l) z (x2,y2) 
                                   where (x2,y2) = coordPac (pacEnd w)
chaseMode (State m w@((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) l) z = chasePac (State m (findGhosts1 w) l) z (x2,y2) 
                                   where (x2,y2) = coordPac (pacEnd w)



-- | Dado um State, o ID e as coordenadas do Pacman, esta função elabora jogadas para o Ghost ir atrás do Pacman
--

scatterMode :: State -> Int -> Play

scatterMode (State m w@((Ghost (GhoState (a,(x,y),b,c,d,e) Dead)):t) l) z = (scatterPac (State m (findGhosts1  (((Ghost (GhoState (a,(x,y),b,c,d,e) Alive )):t))) l) z (x2,y2)) 
                                    where (x2,y2) = coordPac w
scatterMode (State m w@((Pacman (PacState (a,(x,y),b,c,d,e) f g h )):t) l) z = (scatterPac (State m (findGhosts1 w) l) z (x2,y2)) 
                                    where (x2,y2) = coordPac w

-- | Retira as coordenadas do Pacman presente na lista de Players
--
coordPac :: [Player] -> Coords
coordPac ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = (x,y)
coordPac (_:t) = coordPac t
                                                                 
-- | Dá-nos a orientação do pacman de uma lista de players 
oriPac :: [Player] -> Orientation
oriPac ((Pacman (PacState (a,(x,y),b,c,d,e) f g h)):t) = c
oriPac (_:t) = oriPac t


-- | Dado um State, o ID e as coordenadas do Pacman, esta função elabora jogadas para o Ghost perseguir o Pacman
chasePac :: State -> Int -> Coords -> Play
chasePac (State m ((w@(r@(Ghost (GhoState (a,(x1,y1),b,c,d,e) Alive)):t))) l) z (x2,y2) | (x1>=x2) && (whosPiece m U r w == Wall) = Move z R
                                                                                        | (x1>=x2) && (whosPiece m R r w == Wall) = Move z U
                                                                                        | (x1>=x2) && (whosPiece m L r w == Wall) = Move z U
                                                                                        | (x1>=x2) && (whosPiece m D r w == Wall) = Move z U
                                                                                        | (x1<=x2) && (whosPiece m U r w == Wall) = Move z D
                                                                                        | (x1<=x2) && (whosPiece m R r w == Wall) = Move z D
                                                                                        | (x1<=x2) && (whosPiece m L r w == Wall) = Move z D
                                                                                        | (x1<=x2) && (whosPiece m D r w == Wall) = Move z L
                                                                                        | (y1>=y2) && (whosPiece m U r w == Wall) = Move z L
                                                                                        | (y1>=y2) && (whosPiece m R r w == Wall) = Move z L
                                                                                        | (y1>=y2) && (whosPiece m L r w == Wall) = Move z U
                                                                                        | (y1>=y2) && (whosPiece m D r w == Wall) = Move z L
                                                                                        | (y1<=y2) && (whosPiece m U r w == Wall) = Move z R
                                                                                        | (y1<=y2) && (whosPiece m R r w == Wall) = Move z D
                                                                                        | (y1<=y2) && (whosPiece m L r w == Wall) = Move z R
                                                                                        | (y1<=y2) && (whosPiece m D r w == Wall) = Move z R
                                                                                        | x1 == x2 && y1 >= y2 = (Move a L)
                                                                                        | x1 == x2 && y1 <= y2 = (Move a R)
                                                                                        | x1 <= x2 && y1 == y2 = (Move a D) 
                                                                                        | x1 >= x2 && y1 == y2 = (Move a U) 
                                                                                        | y1 <= y2 && x1 <= x2 = (Move a R)
                                                                                        | y1 >= y2 && x1 >= x2 = (Move a L) 
                                                                                        | y1 >= y2 && x1 <= x2 = (Move a L)
                                                                                        | y1 <= y2 && x1 >= x2 = (Move a R)
                                                                                        | otherwise = chasePac (State m t l) z (x2,y2)
chasePac (State m ((w@(x:t))) l) z (x2,y2) = chasePac (State m (t++[x]) l) z (x2,y2)

-- | Devolve um conjunto de jogadas para cada fantasma do labirinto
--
ghostPlay :: State -> [Play]
ghostPlay (State m [(Pacman(PacState (a,(x,y),b,c,d,e) f g h))] l) = []
ghostPlay s@(State m [(Ghost (GhoState (a,(x,y),b,c,d,e) Dead ))] l) = [scatterMode s a]  
ghostPlay s@(State m [(Ghost (GhoState (a,(x,y),b,c,d,e) Alive ))] l) = [chaseMode s a]  
ghostPlay (State m (w@(Ghost (GhoState (a,(x,y),b,c,d,e) Dead )):t) l) = (scatterMode (State m ((Ghost (GhoState (a,(x,y),b,c,d,e) Dead )):t) l) a) : ghostPlay (State m t l)
ghostPlay (State m (q@(Ghost (GhoState (a,(x,y),b,c,d,e) Alive)):t) l) = (chaseMode (State m ((Ghost (GhoState (a,(x,y),b,c,d,e) Alive)):t) l) a) : ghostPlay (State m t l)
ghostPlay (State m (p1@(Pacman(PacState (a,(x,y),b,c,d,e) f g h)):t) l) = ghostPlay (State m (t++[p1]) l)

-- | Dado um State, o ID e as coordenadas do Pacman, esta função elabora jogadas para o Ghost fugir do Pacman
--
scatterPac :: State -> Int -> Coords -> Play
scatterPac (State m ((w@(r@(Ghost (GhoState (a,(x1,y1),b,c,d,e) Dead)):t))) l) z (x2,y2) | (x1>=x2) && (whosPiece m U r w == Wall) = Move z L
                                                                                         | (x1>=x2) && (whosPiece m R r w == Wall) = Move z D
                                                                                         | (x1>=x2) && (whosPiece m L r w == Wall) = Move z D
                                                                                         | (x1>=x2) && (whosPiece m D r w == Wall) = Move z D
                                                                                         | (x1<=x2) && (whosPiece m U r w == Wall) = Move z U
                                                                                         | (x1<=x2) && (whosPiece m R r w == Wall) = Move z U
                                                                                         | (x1<=x2) && (whosPiece m L r w == Wall) = Move z U
                                                                                         | (x1<=x2) && (whosPiece m D r w == Wall) = Move z R                                                                                          | (y1>=y2) && (whosPiece m U r w == Wall) = Move z L
                                                                                         | (y1>=y2) && (whosPiece m R r w == Wall) = Move z R
                                                                                         | (y1>=y2) && (whosPiece m L r w == Wall) = Move z D
                                                                                         | (y1>=y2) && (whosPiece m D r w == Wall) = Move z R
                                                                                         | (y1<=y2) && (whosPiece m U r w == Wall) = Move z L
                                                                                         | (y1<=y2) && (whosPiece m R r w == Wall) = Move z U
                                                                                         | (y1<=y2) && (whosPiece m L r w == Wall) = Move z L
                                                                                         | (y1<=y2) && (whosPiece m D r w == Wall) = Move z L
                                                                                         | x1 == x2 && y1 >= y2 = (Move a L)
                                                                                         | x1 == x2 && y1 <= y2 = (Move a L)
                                                                                         | x1 <= x2 && y1 == y2 = (Move a U) 
                                                                                         | x1 >= x2 && y1 == y2 = (Move a D) 
                                                                                         | y1 <= y2 && x1 <= x2 = (Move a L)
                                                                                         | y1 >= y2 && x1 >= x2 = (Move a R) 
                                                                                         | y1 >= y2 && x1 <= x2 = (Move a R)
                                                                                         | y1 <= y2 && x1 >= x2 = (Move a L)
                                                                                         | otherwise = scatterPac (State m t l) z (x2,y2)
scatterPac (State m (x:t) l) z (x2,y2) = scatterPac (State m (t++[x]) l) z (x2,y2)

 

