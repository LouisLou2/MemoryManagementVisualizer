#ifndef DERIVEDPRIORITYQUEUE_H
#define DERIVEDPRIORITYQUEUE_H
#include <queue>
#include <algorithm>
template <class _Ty, class _Container = std::deque<_Ty>, class _Pr = std::less<typename _Container::value_type>>
class DerivedPriorityQueue : public std::priority_queue<_Ty, _Container, _Pr> {
public:
    bool remove(const  _Ty& value) {
        auto it = std::find(this->c.begin(), this->c.end(), value);
        if (it != this->c.end()) {
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
            return true;
        }
        else {
            return false;
        }
    }
    bool erase(int index) {
        if (index < this->c.size()) {
            this->c.erase(this->c.begin() + index);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
            return true;
        }
        else {
            return false;
        }
    }
    //注意优先队列实质是堆，所以只能保证堆顶元素是最值，所以不能靠获取优先队列内部元素，来寻找第几小（大）的元素,所以获取内部元素的顺序没有意义，故不提供。
    //如果考虑“使用动态数组，每次有元素变动都重新排序”这个方案，每次都要最少O(nlogn)的复杂度，但是顺序读取是O(1+n)=O(n)的复杂度，
    //而优先队列,每次变动元素(指的是顶部元素变动),调整堆是O(logn)的复杂度，(这里不考虑删除非顶部的元素，此时相当于重新建堆，O(n),但是这种情况在这个实验很少)，但是顺序获取，复杂度是O(nlogn+n),
    //综合考量，采用优先队列
    //调用get之前务必要用size检查
    //_Ty get(size_t index){
    //  return *(this->c.begin()+index);
    //}
    void clear(){
        this->c.clear();
    }
    _Ty get_and_pop_back(){
        if (this->c.empty()) {
            // 如果deque为空，返回一个表示无效的值（这里假设-1表示无效值）
            return nullptr;
        }

        // 使用STL中的max_element函数找到deque中的最大值的迭代器
        auto maxIterator = std::max_element(this->c.begin(), this->c.end());

        _Ty maxValue = *maxIterator; // 获取最大值
        this->c.erase(maxIterator); // 从deque中删除最大值
        std::make_heap(this->c.begin(), this->c.end(), this->comp);
        return maxValue; // 返回最大值
    }
    //此方法允许拿到内部的deque数据结构，但是对内部数据结构使用调整后，不能忘记make_heap
    _Container getInnerStructure(){
        return this->c;
    }
    _Ty get_and_pop_back_without_makeheap(){
        if (this->c.empty()) {
            // 如果deque为空，返回一个表示无效的值（这里假设-1表示无效值）
            return nullptr;
        }
        // 使用STL中的max_element函数找到deque中的最大值的迭代器
        auto maxIterator = std::max_element(this->c.begin(), this->c.end());

        _Ty maxValue = *maxIterator; // 获取最大值
        this->c.erase(maxIterator); // 从deque中删除最大值
        return maxValue; // 返回最大值
    }
    void adjust_heap(){
        std::make_heap(this->c.begin(), this->c.end(), this->comp);
    }
    bool empty(){
        return (this->c).empty();
    }
};
#endif // DERIVEDPRIORITYQUEUE_H
