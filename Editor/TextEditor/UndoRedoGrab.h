#ifndef UNDOREDOGRAB_H
#define UNDOREDOGRAB_H

#include "TextSelection.h"

class UndoRedoGrab : public UndoRedoElement
{
private:
    TextSelection preSrcText, preDstText;
    TextSelection postSrcText, postDstText;
    int srcRow, srcCol, dstRow, dstCol;

public:
    UndoRedoGrab(TextSelection preSrcText, TextSelection preDstText,
                 TextSelection postSrcText, TextSelection postDstText,
                 int srcRow, int srcCol, int dstRow, int dstCol)
    {
        this->preSrcText = preSrcText;
        this->preDstText = preDstText;
        this->postSrcText = postSrcText;
        this->postDstText = postDstText;
        this->srcRow = srcRow;
        this->srcCol = srcCol;
        this->dstRow = dstRow;
        this->dstCol = dstCol;

        int preRowBegin = srcRow + 1;
        int preColBegin = srcCol + 1;
        int preRowEnd = preRowBegin + preSrcText.height();
        int preColEnd = preColBegin + preSrcText.width();

        int postRowBegin = dstRow + 1;
        int postColBegin = dstCol + 1;
        int postRowEnd = postRowBegin + postSrcText.height();
        int postColEnd = postColBegin + postSrcText.width();

        displayName = "Grab (" + QString::number(preRowBegin)
                + ", " + QString::number(preColBegin)
                + ", " + QString::number(preRowEnd)
                + ", " + QString::number(preColEnd)
                + ") to (" + QString::number(postRowBegin)
                + ", " + QString::number(postColBegin)
                + ", " + QString::number(postRowEnd)
                + ", " + QString::number(postColEnd) +")";
    }

    void getPre(TextSelection *preSrcText, TextSelection *preDstText,
                int *srcRow, int *srcCol, int *dstRow, int *dstCol) const
    {
        *preSrcText = this->preSrcText;
        *preDstText = this->preDstText;
        *srcRow = this->srcRow;
        *srcCol = this->srcCol;
        *dstRow = this->dstRow;
        *dstCol = this->dstCol;
    }

    void getPost(TextSelection *postSrcText, TextSelection *postDstText,
                int *srcRow, int *srcCol, int *dstRow, int *dstCol) const
    {
        *postSrcText = this->postSrcText;
        *postDstText = this->postDstText;
        *srcRow = this->srcRow;
        *srcCol = this->srcCol;
        *dstRow = this->dstRow;
        *dstCol = this->dstCol;
    }
};

#endif // UNDOREDOGRAB_H
