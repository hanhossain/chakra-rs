//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

class Lifetime
{
public:

    Lifetime(JitArenaAllocator * alloc, StackSym *sym, RegNum reg, uint32_t start, uint32_t end)
        :
        sym(sym),
        regionUseCount(nullptr),
        regionUseCountAdjust(nullptr),
        defList(alloc),
        useList(alloc),
        lastUseLabel(nullptr),
        region(nullptr),
        spillStackSlot(nullptr),
        alloc(alloc),
        intUsageBv(0),
        regPreference(0),
        start(start),
        end(end),
        previousDefBlockNumber(0),
        useCount(0),
        useCountAdjust(0),
        allDefsCost(0),
        lastAllocationStart(0),
        reg(reg),
        totalOpHelperLengthByEnd(0),
        isSpilled(false),
        isLiveAcrossCalls(false),
        isLiveAcrossUserCalls(false),
        isDeadStore(true),
        isOpHelperSpilled(false),
        isOpHelperSpillAsArg(false),
        cantOpHelperSpill(false),
        cantSpill(false),
        dontAllocate(false),
        isSecondChanceAllocated(false),
        isCheapSpill(false),
        needsStoreCompensation(false),
        cantStackPack(false),
        isFloat(false)
    {
    }

public:
    StackSym *          sym;
    uint32_t *            regionUseCount;
    uint32_t *            regionUseCountAdjust;
    SList<IR::Instr *>  defList;
    SList<IR::Instr *>  useList;
    IR::LabelInstr *    lastUseLabel;
    Region *            region;
    StackSlot *         spillStackSlot;
    JitArenaAllocator * alloc;
    BitVector           intUsageBv;
    BitVector           regPreference;
    uint32_t              start;
    uint32_t              end;
    uint32_t              previousDefBlockNumber;
    uint32_t              useCount;
    uint32_t              useCountAdjust;
    uint32_t              allDefsCost;
    uint32_t              lastAllocationStart;
    RegNum              reg;
    uint                totalOpHelperLengthByEnd;
    uint8_t               isSpilled:1;
    uint8_t               isLiveAcrossCalls:1;
    uint8_t               isLiveAcrossUserCalls:1;
    uint8_t               isDeadStore:1;
    uint8_t               isOpHelperSpilled:1;
    uint8_t               isOpHelperSpillAsArg : 1;
    uint8_t               cantOpHelperSpill:1;
    uint8_t               cantSpill:1;
    uint8_t               dontAllocate:1;
    uint8_t               isSecondChanceAllocated:1;
    uint8_t               isCheapSpill:1;
    uint8_t               needsStoreCompensation:1;
    uint8_t               cantStackPack : 1;
    uint8_t               isFloat : 1;

    bool IsInt()
    {
        return !isFloat;
    }

    void AddToUseCount(uint32_t newUseValue, Loop *loop, Func *func)
    {
        Assert((this->useCount + newUseValue) >= this->useCount);
        this->useCount += newUseValue;

        if (loop)
        {
            if (!this->regionUseCount)
            {
                this->regionUseCount = AnewArrayZ(this->alloc, uint32_t, func->loopCount+1);
                this->regionUseCountAdjust = AnewArrayZ(this->alloc, uint32_t, func->loopCount+1);
            }
            while (loop)
            {
                this->regionUseCount[loop->loopNumber] += newUseValue;
                loop = loop->parent;
            }
        }
    }
    void SubFromUseCount(uint32_t newUseValue, Loop *loop)
    {
        Assert((this->useCount - newUseValue) <= this->useCount);
        this->useCount -= newUseValue;

        Assert(!loop || this->regionUseCount);

        while (loop)
        {
            Assert((this->regionUseCount[loop->loopNumber] - newUseValue) <= this->regionUseCount[loop->loopNumber]);
            this->regionUseCount[loop->loopNumber] -= newUseValue;
            loop = loop->parent;
        }
    }
    uint32_t GetRegionUseCount(Loop *loop)
    {
        if (loop && !PHASE_OFF1(Js::RegionUseCountPhase))
        {
            if (this->regionUseCount)
            {
                return this->regionUseCount[loop->loopNumber];
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return this->useCount;
        }
    }
    void AddToUseCountAdjust(uint32_t newUseValue, Loop *loop, Func *func)
    {
        Assert((this->useCountAdjust + newUseValue) >= this->useCountAdjust);
        this->useCountAdjust += newUseValue;

        if (loop)
        {
            if (!this->regionUseCount)
            {
                this->regionUseCount = AnewArrayZ(this->alloc, uint32_t, func->loopCount+1);
                this->regionUseCountAdjust = AnewArrayZ(this->alloc, uint32_t, func->loopCount+1);
            }
            do
            {
                this->regionUseCountAdjust[loop->loopNumber] += newUseValue;
                loop = loop->parent;
            } while (loop);
        }
    }
    void ApplyUseCountAdjust(Loop *loop)
    {
        Assert((this->useCount + this->useCountAdjust) >= this->useCount);
        this->useCount -= this->useCountAdjust;
        this->useCountAdjust = 0;

        if (loop && this->regionUseCount)
        {
            do
            {
                Assert((this->regionUseCount[loop->loopNumber] - this->regionUseCountAdjust[loop->loopNumber]) <= this->regionUseCount[loop->loopNumber]);
                this->regionUseCount[loop->loopNumber] -= this->regionUseCountAdjust[loop->loopNumber];
                this->regionUseCountAdjust[loop->loopNumber] = 0;
                loop = loop->parent;
            } while (loop);
        }
    }
};

