#ifndef LR1_EXPLORER_TREEMODEL_HPP
#define LR1_EXPLORER_TREEMODEL_HPP

#include <QAbstractItemModel>
#include <vector>

#include <QVariantList>

class TreeItem {
    public:
        explicit TreeItem(QVariantList data, TreeItem *parentItem = nullptr);

        void appendChild(std::unique_ptr<TreeItem> &&child);

        TreeItem *child(int row);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        int row() const;
        TreeItem *parentItem();

    private:
        std::vector<std::unique_ptr<TreeItem>> m_childItems;
        QVariantList m_itemData;
        TreeItem *m_parentItem;
};

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

    public:
        Q_DISABLE_COPY_MOVE(TreeModel)

        explicit TreeModel(const QString &data, QObject *parent = nullptr);
        ~TreeModel() override;

        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    private:
        static void setupModelData(const QList<QStringView> &lines, TreeItem *parent);

        std::unique_ptr<TreeItem> rootItem;
};

#endif //LR1_EXPLORER_TREEMODEL_HPP
