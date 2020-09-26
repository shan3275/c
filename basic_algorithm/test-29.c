/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#define MAXSIZE 50001
#define MAXSIZE1 5000
int GetMin(int x, int y)
{
    if (x > y) {
        return y;
    }
    return x;
}
int* intersect(int* nums1, int nums1Size, int* nums2, int nums2Size, int* returnSize){
   int arr1[MAXSIZE] = {0};
    *returnSize = 0;
    int len = GetMin(nums1Size, nums2Size) + 1;
    int *res = (int *)malloc(sizeof(int) * len);
    
    for (int i = 0; i < nums1Size; i++) {
        int tmp = nums1[i] % MAXSIZE1;
        if (tmp < 0) {
            tmp = tmp + MAXSIZE;
        }
        nums1[i] = tmp;
        arr1[nums1[i]]++;
    }
    for (int i = 0; i < nums2Size; i++) {
        int tmp = nums2[i] % MAXSIZE1;
        if (tmp < 0) {
            tmp = tmp + MAXSIZE;
        }
        if (arr1[tmp] >= 1) {
            res[(*returnSize)++] = nums2[i];
            arr1[tmp]--;
        }
    }
    return res;
}