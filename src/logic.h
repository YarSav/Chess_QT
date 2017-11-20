#pragma once
#include <memory>
#include <QAbstractListModel>

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:



    enum GlobalConstants {
        BOARD_SIZE = 8,
        FIGURES_COUNT = 32,

        WHITE_PAWN = 0,
        BLACK_PAWN = 1,
        WHITE_ROOK = 2,
        BLACK_ROOK = 3,
        WHITE_KNIGHT = 4,
        BLACK_KNIGHT = 5,
        WHITE_BISHOP = 6,
        BLACK_BISHOP = 7,
        WHITE_KING = 8,
        BLACK_KING = 9,
        WHITE_QUEEN = 10,
        BLACK_QUEEN = 11,

        KING_B = 30,
        KING_W = 14


    };

    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
        ActiveF,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT)
    int boardSize() const;


    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void makeNewField();
    Q_INVOKABLE bool isMyTurn(int a);
    Q_INVOKABLE bool makeNextStep();
    Q_INVOKABLE bool makePrevStep();
    Q_INVOKABLE bool isWKing();
    Q_INVOKABLE bool isBKing();
    bool castling();

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    int scene;
    struct Impl;
    int     _turn;
    std::unique_ptr<Impl> impl;
};
