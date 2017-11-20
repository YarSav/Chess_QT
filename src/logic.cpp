#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>

struct Figure
{
  int type;
  int x;
  int y;
  int activeF;
};
struct Move
{
    int index;
    int oldX;
    int oldY;
    int nX;
    int nY;
};

struct Logic::Impl
{
  QList<Figure> figures;
  QList<Move> moves;
  int cheking;

  int findByPosition(int x, int y);
  bool isCorrectMove(int index, int toX, int toY);
  bool pawnMove(Figure *figure, int toX, int toY, int player);
  bool knightMove(Figure *figure, int toX, int toY);
  bool rookMove(Figure *figure, int toX, int toY);
  bool bishopMove(Figure *figure, int toX, int toY);
  bool queenMove(Figure *figure, int toX, int toY);
  bool kingMove(Figure *figure, int toX, int toY);
};

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) { 
      continue; 
    }
    return i;    
  }
  return -1;
}

Logic::Logic(QObject *parent)
  : QAbstractListModel(parent)
  , impl(new Impl()) 
{
    for (int i = 0; i < 32; ++i)
        impl->figures << Figure();
  _turn = 0;
  scene = 0;
  makeNewField();
}

Logic::~Logic() {
    impl->figures.clear();
    impl->moves.clear();
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Type      , "type");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  names.insert(Roles::ActiveF , "isActive");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
  if (!modelIndex.isValid()) {
    return QVariant();
  }
    
  int index = static_cast<int>(modelIndex.row());
  
  if (index >= impl->figures.size() || index < 0) {
    return QVariant();
  }

  Figure & figure = impl->figures[index];
    
  switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
    case Roles::ActiveF:   return figure.activeF;
  }
  return QVariant(); 
}

void Logic::clear() {
     impl->moves.clear();
}

void Logic::makeNewField()
{
    _turn = 0;
    impl->figures[0] = Figure { WHITE_PAWN, 0, 1 ,1 };
    impl->figures[1] = Figure { WHITE_PAWN, 1, 1 ,1 };
    impl->figures[2] = Figure { WHITE_PAWN, 2, 1 ,1 };
    impl->figures[3] = Figure { WHITE_PAWN, 3, 1 ,1 };
    impl->figures[4] = Figure { WHITE_PAWN, 4, 1 ,1 };
    impl->figures[5] = Figure { WHITE_PAWN, 5, 1 ,1 };
    impl->figures[6] = Figure { WHITE_PAWN, 6, 1 ,1 };
    impl->figures[7] = Figure { WHITE_PAWN, 7, 1 ,1 };

    impl->figures[8] = Figure { WHITE_ROOK, 0, 0 ,1 };
    impl->figures[9] = Figure { WHITE_ROOK, 7, 0 ,1 };

    impl->figures[10] = Figure { WHITE_KNIGHT, 1, 0 ,1 };
    impl->figures[11] = Figure { WHITE_KNIGHT, 6, 0 ,1 };

    impl->figures[12] = Figure { WHITE_BISHOP, 2, 0 ,1 };
    impl->figures[13] = Figure { WHITE_BISHOP, 5, 0 ,1 };

    impl->figures[14] = Figure { WHITE_KING, 4, 0 ,1 };

    impl->figures[15] = Figure { WHITE_QUEEN, 3, 0 ,1 };

    impl->figures[16] = Figure { BLACK_PAWN, 0, 6 ,1 };
    impl->figures[17] = Figure { BLACK_PAWN, 1, 6 ,1 };
    impl->figures[18] = Figure { BLACK_PAWN, 2, 6 ,1 };
    impl->figures[19] = Figure { BLACK_PAWN, 3, 6 ,1 };
    impl->figures[20] = Figure { BLACK_PAWN, 4, 6 ,1 };
    impl->figures[21] = Figure { BLACK_PAWN, 5, 6 ,1 };
    impl->figures[22] = Figure { BLACK_PAWN, 6, 6 ,1 };
    impl->figures[23] = Figure { BLACK_PAWN, 7, 6 ,1 };

    impl->figures[24] = Figure { BLACK_ROOK, 0, 7 ,1 };
    impl->figures[25] = Figure { BLACK_ROOK, 7, 7 ,1 };

    impl->figures[26] = Figure { BLACK_KNIGHT, 1, 7 ,1 };
    impl->figures[27] = Figure { BLACK_KNIGHT, 6, 7 ,1 };

    impl->figures[28] = Figure { BLACK_BISHOP, 2, 7 ,1 };
    impl->figures[29] = Figure { BLACK_BISHOP, 5, 7 ,1 };

    impl->figures[30] = Figure { BLACK_KING, 3, 7 ,1 };

    impl->figures[31] = Figure { BLACK_QUEEN, 4, 7 ,1 };
    int end = impl->figures.size();
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(end - 1, 0);

    emit dataChanged (topLeft,bottomRight);
}
bool Logic::makePrevStep(){
    if (_turn == 0)
        return false;
      _turn--;
    Move curr =  impl->moves[_turn*2+1];
    Move curr2 =  impl->moves[_turn*2];
    impl->figures[curr.index].x = curr.oldX;
    impl->figures[curr.index].y = curr.oldY;
    QModelIndex topLeft = createIndex(curr.index, 0);
    QModelIndex bottomRight = createIndex(curr.index, 0);
     emit dataChanged(topLeft, bottomRight);
    if (curr2.index != -1)
       {
            impl->figures[curr2.index].activeF = 1;
            topLeft = createIndex(curr2.index, 0);
            bottomRight = createIndex(curr2.index, 0);
            emit dataChanged(topLeft, bottomRight);
        }
    return true;
}
bool Logic::makeNextStep(){
    if (_turn * 2 + 2 > impl->moves.size())
        return false;
    Move curr =  impl->moves[_turn*2+1];
    Move curr2 =  impl->moves[_turn*2];
    impl->figures[curr.index].x = curr.nX;
    impl->figures[curr.index].y = curr.nY;
    QModelIndex topLeft = createIndex(curr.index, 0);
    QModelIndex bottomRight = createIndex(curr.index, 0);
     emit dataChanged(topLeft, bottomRight);
    if (curr2.index != -1)
       {
            impl->figures[curr2.index].activeF = 0;
            topLeft = createIndex(curr2.index, 0);
            bottomRight = createIndex(curr2.index, 0);
            emit dataChanged(topLeft, bottomRight);
        }
      _turn++;
      return true;
}
bool Logic::Impl::pawnMove(Figure *figure, int toX, int toY, int p){
    int index = findByPosition(toX, toY);
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if (index == -1 && ((p && yOffset == 1 && !xOffset) || (!p && yOffset == -1 && !xOffset)))
    {
        moves<< Move {-1,0,0,0,0};
        return true;
    }
    if (index >= 0 && ((p && yOffset == 1 && abs(xOffset) == 1) || (!p && yOffset == -1 && abs(xOffset) == 1) ))
    {
        moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
        figures[index] = Figure{0,8,8,0};
         return true;
    }
    return false;
}
bool Logic::Impl::knightMove(Figure *figure, int toX, int toY){
    int index = findByPosition(toX, toY);
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if ((xOffset == 1 && yOffset ==2) || (xOffset == 2 && yOffset == 1) || (xOffset == -1 && yOffset ==2) || (xOffset == -2 && yOffset == 1)||
            (xOffset == -1 && yOffset == -2) || (xOffset == -2 && yOffset == -1) || (xOffset == 1 && yOffset == -2) || (xOffset == 2 && yOffset == -1))
    {
        if (index >= 0 && figures[index].type%2 != figure->type%2)
        {
            moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
            figures[index] = Figure{0,8,8,0};
             return true;
        }
        if (index == -1)
        {
            moves<< Move{-1,0,0,0,0};
            return true;
        }
    }
    return false;
}
bool Logic::Impl::rookMove(Figure *figure, int toX, int toY){
    int index = findByPosition(toX, toY);
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if (xOffset && yOffset)
        return false;
    int s_x = (xOffset > 0)?1:-1;
    int s_y = (yOffset > 0)?1:-1;
    int startX = figure->x ;
    int startY = figure->y ;
    if (xOffset){
        while (startX != toX){
           index =  findByPosition(startX+ s_x, startY);
           if (index >= 0)
           {
               if (figures[index].type % 2 == figure->type %2)
                   return false;
               if (startX + s_x== toX)
               {
                   moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
                   figures[index] = Figure{0,8,8,0};
                    return true;
               }
               else
                   return false;
           }
           startX+= s_x;
        }
    }
    else if (yOffset){
        while (startY != toY){
           index =  findByPosition(startX, startY+ s_y);
           if (index >= 0)
           {
               if (figures[index].type % 2 == figure->type %2)
                   return false;
               if (startY + s_y== toY)
               {
                   moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
                   figures[index] = Figure{0,8,8,0};
                    return true;
               }
               else
                   return false;
           }
           startY+= s_y;
        }
    }
     moves<< Move{-1,0,0,0,0};
    return true;
}
bool Logic::Impl::bishopMove(Figure *figure, int toX, int toY){
    int index;
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if (abs(yOffset)!=abs(xOffset))
        return false;
    int s_x = (xOffset > 0)?1:-1;
    int s_y = (yOffset > 0)?1:-1;
    int startX = figure->x ;
    int startY = figure->y ;
    while (startX != toX){
       index =  findByPosition(startX+ s_x, startY+ s_y);
       if (index >= 0)
       {
           if (figures[index].type % 2 == figure->type %2)
           {
               return false;
           }
           if (startX + s_x== toX)
           {
               moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
               figures[index] = Figure{0,8,8,0};
                return true;
           }
           else
               return false;
       }
       startX+= s_x;
       startY+= s_y;
    }
     moves<< Move{-1,0,0,0,0};
    return true;
}
bool Logic::Impl::queenMove(Figure *figure, int toX, int toY){
    int index;
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if (abs(yOffset)!=abs(xOffset) && xOffset && yOffset)
        return false;
    int s_x = (xOffset > 0)?1:-1;
    int s_y = (yOffset > 0)?1:-1;
    int startX = figure->x ;
    int startY = figure->y ;
    if (abs(yOffset)==abs(xOffset))
        while (startX != toX){
           index =  findByPosition(startX+ s_x, startY+ s_y);
           if (index >= 0)
           {
               if (figures[index].type % 2 == figure->type %2)
                   return false;
               if (startX + s_x== toX)
               {
                   moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
                   figures[index] = Figure{0,8,8,0};
                   return true;
               }
               else
                   return false;
           }
           startX+= s_x;
           startY+= s_y;
        }
    else if (xOffset){
        while (startX != toX){
           index =  findByPosition(startX+ s_x, startY);
           if (index >= 0)
           {
               if (figures[index].type % 2 == figure->type %2)
                   return false;
               if (startX + s_x== toX)
                {
                   moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
                   figures[index] = Figure{0,8,8,0};
                   return true;
               }
               else
                   return false;
           }
           startX+= s_x;
        }
    }
    else if (yOffset){
        while (startY != toY){
           index =  findByPosition(startX, startY+ s_y);
           if (index >= 0)
           {
               if (figures[index].type % 2 == figure->type %2)
                   return false;
               if (startY + s_y== toY)
                {
                   moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
                   figures[index] = Figure{0,8,8,0};
                   return true;
               }
               else
                   return false;
           }
           startY+= s_y;
        }
    }
         moves<< Move{-1,0,0,0,0};
    return true;
}
bool Logic::Impl::kingMove(Figure *figure, int toX, int toY){
    int index = findByPosition(toX, toY);
    int xOffset = toX - figure->x;
    int yOffset = toY - figure->y;
    if (!(xOffset <= 1 && xOffset >= -1 && yOffset >= -1 && yOffset <= 1))
        return false;
    if (index == -1 )
    {
        moves<< Move{-1,0,0,0,0};
        return true;
    }
    if (figures[index].type%2 == figure->type%2)
        return false;
         moves<< Move{index,figure[index].x,figure[index].y,figure[index].x,figure[index].y};
         figures[index] = Figure{0,8,8,0};
    return true;
}

bool Logic::Impl::isCorrectMove(int index,int toX, int toY){
  Figure current = figures[index];
    int player = (current.type%2 == 0)?1:0;
    bool canMove = true;
    if (current.type == WHITE_PAWN || current.type == BLACK_PAWN)
        canMove = pawnMove(&figures[index], toX, toY, player);
    else if (current.type == WHITE_KNIGHT || current.type == BLACK_KNIGHT)
        canMove = knightMove(&figures[index], toX, toY);
    else if (current.type == WHITE_ROOK || current.type == BLACK_ROOK)
        canMove = rookMove(&figures[index], toX, toY);
    else if (current.type == WHITE_BISHOP || current.type == BLACK_BISHOP)
        canMove = bishopMove(&figures[index], toX, toY);
    else if (current.type == WHITE_QUEEN || current.type == BLACK_QUEEN)
        canMove = queenMove(&figures[index], toX, toY);
    else if (current.type == WHITE_KING || current.type == BLACK_KING)
        canMove = kingMove(&figures[index], toX, toY);
    if (canMove)
       moves<< Move{index, current.x, current.y, toX, toY};
    return canMove;
}

bool Logic::move(int fromX, int fromY, int toX, int toY) {

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(impl->figures.size() - 1, 0);
     int index = impl->findByPosition(fromX, fromY);
    if ((fromX == toX && fromY == toY)||
        index < 0 ||toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE)
    {
        emit dataChanged(topLeft, bottomRight);
        return false;
    }
    if (!impl->isCorrectMove(index, toX, toY))
    {
      emit dataChanged(topLeft, bottomRight);
      return false;
    }
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    _turn++;
    emit dataChanged(topLeft, bottomRight);
    return true;
}

bool Logic::isMyTurn(int a){
    if (!impl->figures[KING_W].activeF || !impl->figures[KING_B].activeF)
        return false;
    if ((a % 2 == 0 && _turn%2 == 0)||(a % 2 != 0 && _turn%2 != 0))
        return true;
    return false;
}

bool Logic::isWKing(){
    return impl->figures[KING_W].activeF;
}

bool Logic::isBKing(){
    return impl->figures[KING_B].activeF;
}


